#include<bits/stdc++.h>
using namespace std;

typedef int HashType;
typedef int KeyType;
typedef int ValueType;


struct Tuple{
   KeyType key;
   ValueType val;
};

struct Node{
   Tuple* entry;//pointer to tuple
   KeyType key;
   Node* leftChild;
   Node* rightChild;
   HashType hash;
   int reference_counter;
   //int depth;
   Node(Tuple* entry){
       this->entry=entry;
       leftChild= nullptr;
       rightChild= nullptr;
       key=entry->key;
       reference_counter=1;
       //depth=1;
       hash=rand();
   }
};

int height(const Node* root){//求树高，递归
   if (root == nullptr)
       return 0;
   return max(height(root->leftChild) ,height(root->rightChild)) + 1;
}

Node* constructBinaryTree(Tuple tuples[],int  left,int right){
   if(left>right)return nullptr;
   int mid=(left+right)/2;
   Node* root=new Node(new Tuple(tuples[mid]));
   root->key=tuples[mid].key;
   root->leftChild= constructBinaryTree(tuples,left,mid-1);
   root->rightChild= constructBinaryTree(tuples,mid+1,right);
   return root;
}

//构建p-树
Node* build(Tuple A[],int m){
   return constructBinaryTree(A,0,m-1);
}

//二分搜索
int binarySearch(Tuple A[],int m,int targetkey){
   int left=0,right=m-1;
   while (left<=right){
       int mid=(left+right)>>1;
       if(A[mid].key <= targetkey){
           left=mid+1;
       }else{
           right=mid-1;
       }
   }
   return right;
}

//复制节点
Node* copy(Node* t){
   return  new Node(*t);
}

//连接父子节点
Node* concat(Node* L,Node* t,Node* R){
   t->leftChild=L;
   t->rightChild=R;
   return t;
}

//引用计数器加1
void add_ref(Node* t){
   t->reference_counter++;
}

Node* right(Node* t,int k){
   if(t== nullptr)return t;
   if(k>t->key)return right(t->rightChild,k);
   Node* t2=copy(t);
   add_ref(t->rightChild);
   return concat(right(t->leftChild,k),t2,t->rightChild);
}
Node* left(Node* t,int k){
   if(t== nullptr)return t;
   if(k<t->key)return right(t->leftChild,k);
   Node* t2=copy(t);
   add_ref(t->leftChild);
   return concat(left(t->rightChild,k),t2,t->leftChild);
}

//返回子树节点数目
int size(Node* t){
   Node* cur=t;
   int counter=0;
   while (cur!= nullptr){
       if(cur->leftChild!= nullptr){//如果左子树非空
           //先找到中序遍历的前驱节点，注意，是中序；也即左子树的最右节点
           Node* pre=cur->leftChild;
           while (cur->rightChild!= nullptr&&pre->rightChild!=cur)
               pre=pre->rightChild;
           if(pre->rightChild== nullptr){//若前驱的右指针为空，则说明cur还没被遍历,先输出cur
               counter++;
               pre->rightChild=cur;
               cur=cur->leftChild;
           } else{//若前驱的右指针为指向cur，说明cur被遍历过
               pre->rightChild= nullptr;
               cur=cur->rightChild;
           }
       } else{//如果左子树为空，则直接遍历cur，并向右子树走
           counter++;
           cur=cur->rightChild;
       }
   }
   return counter;
}

//t:根节点;A[]:待插入的元组(已排序);m:元组个数
Node* multi_insert(Node* t,Tuple A[],int m,int (*rho)(int val1,int val2)){
   //若p树为空，则构建p树
   if(t== nullptr) return build(A,m);
   if(m==0){
       add_ref(t);
       return t;
   }
   int b= binarySearch(A,m,t->key);
   int d=(b<m&&A[b].key>t->key)?1:0;
   Node* L= multi_insert(t->leftChild,A,b,rho);
   Node* R= multi_insert(t->rightChild,A+b-d,m-b-d,rho);
   Node* t2=copy(t);
   if(d==1)t2->entry->val=rho(t->entry->val,A[b].val);
   return concat(L,t2,R);
}

Node* range(Node* t,int kl,int kr){
   Node* cur=t;
   while(cur->key<kl||cur->key>kr){
       if(cur->key<kl)cur=cur->rightChild;
       if(cur->key>kr)cur=cur->leftChild;
   }
   Node* L= right(cur->leftChild,kl);
   Node* R=left(cur->rightChild,kr);
   Node* mid= copy(cur);
   return concat(L,mid,R);
}

Node* filter(Node* t,bool (*function)(Node* t)){
   if(t== nullptr)return nullptr;
   Node* L= filter(t->leftChild,function);
   Node* R= filter(t->rightChild,function);
   if(function){
       return concat(L, copy(t),R);
   } else{
       return concat(L, nullptr,R);
   }
}

void foreach_index(Node* t,void (*function)(Tuple* e,int i),int s){
   if(t== nullptr)return;
   int left=size(t->leftChild);
   foreach_index(t->leftChild,function,s);
   foreach_index(t->rightChild,function,s+left+1);
   function(t->entry,left);
}

//先序遍历p-树
void travel(Node* t){
   if(t== nullptr)return;
   cout<<"["<<t->key<<"<"<<t->entry->val<<"] ";
   travel(t->leftChild);
   travel(t->rightChild);
}


int rho(int val1,int val2){return val2;}

void test_build(){
   int m=5;
   Tuple tuples[m];
   cout<<"build p-tree"<<endl;
   KeyType build_key[]={1,2,3,4,5};
   ValueType build_value[]={101,102,103,104,105};

   for(int i=0;i<m;i++){
       tuples[i].key=build_key[i];
       tuples[i].val=build_value[i];
       cout<<"["<<tuples[i].key<<"<"<<tuples[i].val<<"] ";
   }
   cout<<endl;

   Node* indexRoot= nullptr;
   indexRoot= multi_insert(indexRoot,tuples,m,rho);

   cout<<"pre order trvel"<<endl;
   travel(indexRoot);

}

void test_multi_insert(){
   int m=5;
   Tuple tuples[m];
   cout<<"build p-tree"<<endl;
   for(int i=0;i<m;i++){
       tuples[i].key=i;
       tuples[i].val=i;
       cout<<"["<<tuples[i].key<<"<"<<tuples[i].val<<"] ";
   }
   cout<<endl;

   Node* indexRoot= nullptr;
   indexRoot= multi_insert(indexRoot,tuples,5,rho);

   cout<<"pre order trvel"<<endl;
   travel(indexRoot);
}

int main()
{
   test_build();
   return 0;
}

