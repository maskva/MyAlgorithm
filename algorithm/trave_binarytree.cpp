//
// Created by SMS on 2022/4/18.
//
#include <iostream>
using namespace std;

typedef struct Node{
    int value;
    Node* leftChild;
    Node* rightChild;
}Node;


/************************Morris遍历******************************/
//从node出发，一直向右前进直到null,将节点反转
Node* reverseNode(Node* node){
    Node* pre = nullptr;
    Node* next = nullptr;
    while(node!=nullptr){
        next = node->rightChildright;
        node->rightChild = pre;
        pre = node;
        node = next;
    }
    return pre;
}

//从node出发，一直向右前进直到null,逆序输出
void printNode(Node* node){
    Node* tail = reverseNode(node);
    Node* cur = tail;
    while(cur!=null){
        cout<<cur->value<<" ";
        cur = cur->rightChild;
    }
    reverseNode(tail);
}

//Morris前序遍历
void MorrisPreOrder(Node* root){
    Node* cur=root;
    while (cur!= nullptr){
        if(cur->leftChild!= nullptr){//如果左子树非空
            //先找到中序遍历的前驱节点，注意，是中序；也即左子树的最右节点
            Node* pre=cur->leftChild;
            while (cur->rightChild!= nullptr&&pre->rightChild!=cur)
                pre=pre->rightChild;
            if(pre->rightChild== nullptr){//若前驱的右指针为空，则说明cur还没被遍历,先输出cur，再向左走
                cout<<cur->value<<" ";
                pre->rightChild=cur;
                cur=cur->leftChild;
            } else{//若前驱的右指针为指向cur，说明cur和左子树被遍历过，向右子树走
                pre->rightChild= nullptr;
                cur=cur->rightChild;
            }
        } else{//如果左子树为空，则直接遍历cur，并向右子树走，也可能是返回某个之前遍历过的节点
            cout<<cur->value<<" ";
            cur=cur->rightChild;
        }
    }
}

//Morris中序遍历
void MorrisInOrder(Node* root){
    Node* cur=root;
    while (cur!= nullptr){
        if(cur->leftChild!= nullptr){//如果左子树非空
            //先找到中序遍历的前驱节点
            Node* pre=cur->leftChild;
            while (cur->rightChild!= nullptr&&pre->rightChild!=cur)
                pre=pre->rightChild;
            if(pre->rightChild== nullptr){//若pre的右指针为空，则说明pre还没被遍历
                pre->rightChild=cur;
                cur=cur->leftChild;
            } else{//若pre的右指针指向cur，说明pre被遍历过,可以输出cur
                cout<<cur->value<<" ";
                pre->rightChild= nullptr;
                cur=cur->rightChild;
            }
        } else{//如果左子树为空，则直接遍历cur，并向右子树走
            cout<<cur->value<<" ";
            cur=cur->rightChild;
        }
    }
}

//Morris后序遍历
void MorrisInOrder(Node* root){
    Node* cur=root;
    while (cur!= nullptr){
        if(cur->leftChild!= nullptr){//如果左子树非空
            //先找到中序遍历的前驱节点
            Node* pre=cur->leftChild;
            while (cur->rightChild!= nullptr&&pre->rightChild!=cur)
                pre=pre->rightChild;
            if(pre->rightChild== nullptr){//若中序前驱的右指针为空，则说明中序前驱还没被遍历
                pre->rightChild=cur;
                cur=cur->leftChild;
            } else{//若中序前驱的右指针指向cur，说明cur是从中序前驱那走过一遍再回来的
                pre->rightChild= nullptr;
                printNode(cur->leftChild);//注意，这里的顺序一定不能错
                cur=cur->rightChild;
            }
        } else{//如果左子树为空，则直接向右走
            cur=cur->rightChild;
        }
    }
}

/************************栈模拟遍历******************************/
