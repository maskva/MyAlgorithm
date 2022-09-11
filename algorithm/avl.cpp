//
// Created by SMS on 2022/7/2.
//
#include <iostream>
using namespace std;

typedef int ElenmentType;
typedef struct AVLNode{
    int depth;//深度
    struct AVLNode *left;
    struct AVLNode *right;
    struct AVLNode *parent;
    ElenmentType value;
    //构造器
    explicit AVLNode(ElenmentType value=0){
        parent = nullptr;
        depth = 0;
        left = right = nullptr;
        this->value=value;
    }
}AVLtree,Tree;

//查找value，成功则返回该结点
Tree* binaryTreeSearch(Tree *node,int value){
    if(node== nullptr)return nullptr;
    if(node->value==value)
        return node;
    else if(node->value>value){
        if(node->left!= nullptr)
            return binaryTreeSearch(node->left,value);
        else return nullptr;
    }else{
        if(node->right!=nullptr)
            return binaryTreeSearch(node->right,value);
        else
            return nullptr;
    }
}

//LL型调整函数
//返回根结点
Tree* LL_rotate(Tree *root){//LL执行右旋
    //root是原来的平衡二叉树的根结点
    Tree *temp;//临时变量

    //获取根结点的左孩子
    temp = root->left;
    //根结点的左孩子变更为其原来左孩子的右孩子
    root->left = temp->right;

    //原来的根结点的左孩子变为了根结点
    temp->right = root;
    return temp;

}

//RR型调整函数
//返回根结点
Tree* RR_rotate(Tree * root){//RR执行左旋
    Tree* temp;
    temp = root->right;//获取根结点的右孩子
    root->right = temp->left;//根结点的右孩子变为其原来右孩子的左孩子
    temp->left = root;//原来的根结点的右孩子变为了新的根结点
    return temp;
}

//1、LR型，先左旋转，再右旋转
//返回：新父节点
Tree* LR_rotate(Tree* root){
    Tree* temp;
    temp = root->left;
    root->left =RR_rotate(temp);
    return LL_rotate(root);
}
//2RL型，先右旋转，再左旋转
//返回:新父节点
Tree* RL_rotate(Tree* root){
    Tree* temp;
    temp = root->right;
    root->right=LL_rotate(temp);
    return RR_rotate(root);
}

//树高
int height(const Tree* root){//求树高，递归
    if (root == nullptr)
        return 0;
    return std::max(height(root->left) ,height(root->right)) + 1;
}
//平衡因子
int diff(const Tree* root){//求平衡因子，即当前节点左右子树的差
    return height(root->left) - height(root->right);
}

//平衡操作
Tree* Balance(Tree* root){
    //	printf("平衡函数");
    int balanceFactor = diff(root);//diff用来计算平衡因子（左右子树高度差）
    if (balanceFactor > 1){//左子树高于右子树
        if (diff(root->left) > 0)//LL的情况
            root=LL_rotate(root);
        else//LR的情况
        root=LR_rotate(root);
    }
    else if (balanceFactor < -1){//右子树高于左子树
        if (diff(root->right) > 0)//RL的情况
            root=RL_rotate(root);
        else//RR的情况
        root=RR_rotate(root);
    }
    return root;
}

//插入结点
Tree* Insert(Tree* root,ElenmentType k ){
    if (nullptr == root){
        root = new AVLNode(k);//如果根结点为null，则直接将值为根结点
        if(root==nullptr)
            printf("创建失败");
        return root;
    }//递归返回条件
    else if(binaryTreeSearch(root,k)!= nullptr){
        cout<<k<<"已存在"<<endl;
        return root;
    }else if (k < root->value){
        root->left = Insert(root->left, k);//递归左子树
        //balance operation
        root = Balance(root);//平衡操作包含了四种旋转
    }
    else if (k>root->value){
        root->right = Insert(root->right, k);//递归右子树
        //balance operation
        root = Balance(root);//平衡操作包含了四种旋转
    }
    return root;
}

void showAVL_mid_order(Tree* root){
    if(root== nullptr) {
        //cout<<"# ";
        return;
    }
    showAVL_mid_order(root->left);
    cout<<root->value<<" ";
    showAVL_mid_order(root->right);
}
void showAVL_pre_order(Tree* root){
    if(root== nullptr) {
        //cout<<"# ";
        return;
    }

    cout<<root->value<<" ";
    showAVL_pre_order(root->left);
    showAVL_pre_order(root->right);
}

ElenmentType minInTree(Tree* root){
    Tree* p=root;
    while(p->left){
        p=p->left;
    }
    return p->value;
}
ElenmentType maxInTree(Tree* root){
    Tree* p=root;
    while(p->right){
        p=p->right;
    }
    return p->value;
}

//删除结点
Tree* Delete(Tree *root, const ElenmentType k){
    if (nullptr == root)
        return root;
    if (!binaryTreeSearch(root,k)){//查找删除元素是否存在
        std::cerr << "Delete error , key not find" << std::endl;
        return root;
    }

    if (k == root->value){//根节点
        if (root->left!= nullptr&&root->right!=nullptr){//左右子树都非空
            if (diff(root) > 0){//左子树更高，在左边删除
                root->value = maxInTree(root->left);//以左子树的最大值替换当前值
                root->left = Delete(root->left, root->value);//删除左子树中已经替换上去的节点
            }
            else{//右子树更高，在右边删除
                root->value = minInTree(root->right);
                root->right = Delete(root->right, root->value);
            }
        }
        else{//有一个孩子、叶子节点的情况合并
            Tree * tmp = root;
            root = (root->left) ? (root->left) :( root->right);
            delete tmp;
            tmp = nullptr;
        }
    }
    else if (k < root->value){//往左边删除
        root->left = Delete(root->left, k);//左子树中递归删除
        //判断平衡的条件与在插入时情况类似
        if (diff(root) < -1){//不满足平衡条件，删除左边的后，右子树变高
            if (diff(root->right) > 0){
                root = RL_rotate(root);
            }else{
                root = RR_rotate(root);
            }
        }
    }else{
        root->right = Delete(root->right, k);
        if (diff(root) > 1){//不满足平衡条件
            if (diff(root->left) < 0){
                root = LR_rotate(root);
            }
            else{
                root = LL_rotate(root);
            }
        }
    }
    return root;
}


int main(){
    ElenmentType nums[]={1,2,3,4,5,6,7,8,9,10};
    Tree *root= nullptr;
    for(ElenmentType a:nums){
        root=Insert(root,a);
        showAVL_mid_order(root);
        cout<<endl;
    }
    showAVL_pre_order(root);
    cout<<endl;
    cout<<"======================"<<endl;
    ElenmentType nums2[]={4,3,8,1};
    for(ElenmentType a:nums2){
        root=Delete(root,a);
        showAVL_mid_order(root);
        cout<<endl;
    }
    showAVL_pre_order(root);
    return 0;
}
