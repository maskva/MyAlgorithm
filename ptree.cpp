#include<iostream>
using namespace std;
typedef int HashType;
typedef int KeyType;
typedef int ValueType;

struct Tuple {
    KeyType key;
    ValueType val;
    Tuple() {}
    Tuple(KeyType key, ValueType val) {
        this->key = key;
        this->val = val;
    }
};

struct Node {
    Tuple* entry;//pointer to tuple
    KeyType key;
    Node* lc;
    Node* rc;
    HashType hash;
    int reference_counter;
    int height;
    Node(Tuple* entry, Node* lc, Node* rc) {
        this->entry = entry;
        this->lc = lc;
        this->rc = rc;
        key = entry->key;
        reference_counter = 1;
        height = 1;
        hash = rand() % 100;
    };

};

int rho(ValueType val1, ValueType val2) { return val2; }
bool function(Node* t) {
    return t->key % 3 == 0;
}

//复制节点
Node* copy(Node* t) {
    Node* temp = new Node(*t);
    temp->lc = nullptr;
    temp->rc = nullptr;
    return  temp;
}
//返回子树节点数目 Morris先序
int size(Node* t) {
    Node* cur = t;
    int counter = 0;
    while (cur != nullptr) {
        if (cur->lc != nullptr) {//如果左子树非空
            //先找到中序遍历的前驱节点，注意，是中序；也即左子树的最右节点
            Node* pre = cur->lc;
            while (pre->rc != nullptr && pre->rc != cur)
                pre = pre->rc;
            if (pre->rc == nullptr) {//若前驱的右指针为空，则说明cur还没被遍历,先输出cur
                counter++;
                pre->rc = cur;
                cur = cur->lc;
            }
            else {//若前驱的右指针为指向cur，说明cur被遍历过
                pre->rc = nullptr;
                cur = cur->rc;
            }
        }
        else {//如果左子树为空，则直接遍历cur，并向右子树走
            counter++;

            cur = cur->rc;
        }
    }
    return counter;
}

int getHeight(const Node* root) {
    if (root == nullptr)return 0;
    return root->height;
}
HashType getHash(const Node* root) {
    if (root == nullptr)return 0;
    return root->hash;
}
int updateHeight(Node* root) {//递归更新所有节点的height
    if (root == nullptr)
        return 0;
    root->height = max(updateHeight(root->lc), updateHeight(root->rc)) + 1;
    return root->height;
}
//更新left spine 或者right spine的height,true:right  false:left
int updateHeight(Node* root, Node* targetNode, bool to_right) {
    if (root == targetNode)
        return root->height;
    if (to_right) {
        root->height = max(getHeight(root->lc), updateHeight(root->rc, targetNode, true)) + 1;
    }
    else {
        root->height = max(getHeight(root->rc), updateHeight(root->lc, targetNode, false)) + 1;
    }
    return root->height;
}
HashType calculateHash(HashType hash1, HashType hash2) {
    return hash1 + hash2;
}
HashType updateHash(Node* root) {//递归更新所有节点的height
    if (root == nullptr)
        return 0;
    if (root->lc == nullptr && root->rc == nullptr)//叶子节点直接返回自己的hash
        return root->hash;
    root->hash = calculateHash(updateHash(root->lc), updateHash(root->rc));
    return root->hash;
}
HashType updateHash(Node* root, Node* targetNode, bool to_right) {
    if (root == targetNode)
        return root->hash;
    if (to_right) {//
        root->height = calculateHash(getHash(root->lc), updateHash(root->rc, targetNode, true));
    }
    else {
        root->height = calculateHash(getHash(root->rc), updateHash(root->lc, targetNode, false));
    }
    return root->height;
}

//avl操作
Node* L_rotation(Node* root) {
    Node* newRoot = root->rc;
    root->rc = newRoot->lc;
    newRoot->lc = root;
    root->height = max(getHeight(root->lc), getHeight(root->rc)) + 1;
    root->hash = calculateHash(getHash(root->lc), getHash(root->rc));
    newRoot->height = max(getHeight(newRoot->lc), getHeight(newRoot->rc)) + 1;
    newRoot->hash = calculateHash(getHash(newRoot->lc), getHash(newRoot->rc));
    return newRoot;
}
Node* R_rotation(Node* root) {
    Node* newRoot = root->lc;
    root->lc = newRoot->rc;
    newRoot->rc = root;
    root->height = max(getHeight(root->lc), getHeight(root->rc)) + 1;
    root->hash = calculateHash(getHash(root->lc), getHash(root->rc));
    newRoot->height = max(getHeight(newRoot->lc), getHeight(newRoot->rc)) + 1;
    newRoot->hash = calculateHash(getHash(newRoot->lc), getHash(newRoot->rc));
    return newRoot;
}
Node* LR_rotation(Node* root) {
    Node* newRoot = root->lc->rc;
    root->lc->rc = newRoot->lc;
    newRoot->lc = root->lc;
    root->lc = newRoot->rc;
    newRoot->rc = root;
    newRoot->lc->height = max(getHeight(newRoot->lc->lc), getHeight(newRoot->lc->rc)) + 1;
    newRoot->rc->height = max(getHeight(newRoot->rc->lc), getHeight(newRoot->rc->rc)) + 1;
    newRoot->height = max(newRoot->lc->height, newRoot->rc->height) + 1;//这种情况左右子节点一定存在，直接访问
    newRoot->lc->hash = calculateHash(getHash(newRoot->lc->lc), getHash(newRoot->lc->rc));
    newRoot->rc->hash = calculateHash(getHash(newRoot->rc->lc), getHash(newRoot->rc->rc));
    newRoot->hash = calculateHash(getHash(newRoot->lc), getHash(newRoot->rc));
    return newRoot;
}
Node* RL_rotation(Node* root) {
    Node* newRoot = root->rc->lc;
    root->rc->lc = newRoot->rc;
    newRoot->rc = root->rc;
    root->rc = newRoot->lc;
    newRoot->lc = root;
    newRoot->lc->height = max(getHeight(newRoot->lc->lc), getHeight(newRoot->lc->rc)) + 1;
    newRoot->rc->height = max(getHeight(newRoot->rc->lc), getHeight(newRoot->rc->rc)) + 1;
    newRoot->height = max(newRoot->lc->height, newRoot->rc->height) + 1;//这种情况左右子节点一定存在，直接访问
    newRoot->lc->hash = calculateHash(getHash(newRoot->lc->lc), getHash(newRoot->lc->rc));
    newRoot->rc->hash = calculateHash(getHash(newRoot->rc->lc), getHash(newRoot->rc->rc));
    newRoot->hash = calculateHash(getHash(newRoot->lc), getHash(newRoot->rc));
    return newRoot;
}

//根据有序数组构建AVL
Node* constructBinaryTree(Tuple tuples[], int  left, int right) {
    if (left > right)return nullptr;
    int mid = (left + right) / 2;
    Node* root = new Node(new Tuple(tuples[mid]), nullptr, nullptr);
    root->key = tuples[mid].key;
    root->lc = constructBinaryTree(tuples, left, mid - 1);
    root->rc = constructBinaryTree(tuples, mid + 1, right);
    return root;
}
//构建p-树
Node* build(Tuple A[], int m) {
    Node* root = constructBinaryTree(A, 0, m - 1);
    updateHeight(root);
    updateHash(root);
    return root;
}

//二分搜索 大于等于targetkey的最小索引
int binarySearch(Tuple A[], int m, KeyType targetkey) {
    int left = 0, right = m - 1;
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (A[mid].key >= targetkey) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return left;
}
//binarySearch的测试版本
int binarySearch(KeyType A[], int m, KeyType targetkey) {
    int left = 0, right = m - 1;
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (A[mid] >= targetkey) {
            right = mid - 1;

        }
        else {
            left = mid + 1;
        }
    }
    return left;
}


//连接  done
Node* concat(Node* L, Node* t, Node* R) {//上层调用的函数保证了t!=null

    if (getHeight(L) > getHeight(R) + 1) {//若R==null,处理方式一致，不必单独处理
        t->rc = R;
        Node* p = L;
        Node* pre_to_p = nullptr;
        //寻找高为h(R)+1的子树，根节点为p->rc,
        //特殊情况：h(L->l)=h(R)+1,h(L->r)=h(R),此时，该子树的高度为h(R)
        while (getHeight(p->rc) > getHeight(R) + 1) {
            pre_to_p = p;
            p = p->rc;
        }
        t->lc = p->rc;
        p->rc = t;
        t->height = max(getHeight(t->lc), getHeight(t->rc)) + 1;
        //p的左右子树高度分别为h(R),h(R)+1
        if (getHeight(p->lc) < getHeight(t->lc)) {
            p->height = max(getHeight(p->lc), getHeight(p->rc)) + 1;
            if (pre_to_p == nullptr) {//也即p=L，进行RL旋转后，根节点会变
                return RL_rotation(p);
            }
            else {
                pre_to_p->rc = RL_rotation(p);
            }
        }
        //p的左右子树高度都为h(R)+1,(都为h(R)的情况不存在,原因见前面的注释)
        //此时应当更新从root到p路径上节点的height
        else if (getHeight(p->lc) == getHeight(t->lc)) {
            p->height = max(getHeight(p->lc), getHeight(p->rc)) + 1;
            updateHeight(L, p, true);
        }
        updateHash(L, p, true);
        return L;
    }
    else if (getHeight(R) > getHeight(L) + 1) {
        t->rc = L;
        Node* p = R;
        Node* pre_to_p = nullptr;
        while (getHeight(p->lc) > getHeight(L) + 1) {
            pre_to_p = p;
            p = p->lc;
        }
        t->rc = p->lc;
        p->lc = t;
        t->height = max(getHeight(t->lc), getHeight(t->rc)) + 1;
        if (getHeight(p->rc) < getHeight(t->rc)) {
            p->height = max(getHeight(p->lc), getHeight(p->rc)) + 1;
            if (pre_to_p == nullptr) {
                return LR_rotation(p);
            }
            else {
                pre_to_p->lc = LR_rotation(p);
            }
        }
        else if (getHeight(p->rc) == getHeight(t->rc)) {
            p->height = max(getHeight(p->lc), getHeight(p->rc)) + 1;
            updateHeight(R, p, false);
        }
        updateHash(R, p, false);
        return R;

    }
    else {
        t->lc = L;
        t->rc = R;
        t->height = max(getHeight(L), getHeight(R)) + 1;
        t->hash = calculateHash(getHash(L), getHash(R));
        return t;
    }
}

//找到树的最小值节点（最右下）,并递归调整，保持平衡
Node* splitLast(Node* t, Node*& k) {
    if (t->rc == nullptr) {//上层调用的函数保证了t!=null
        Node* L = t->lc;
        k = t;
        k->lc = nullptr;
        k->height = 1;
        return L;
    }
    else
    {
        Node* temp = splitLast(t->rc, k);
        return concat(t->lc, t, temp);
    }
}

//连接
Node* concat(Node* L, Node* R) {
    if (L == nullptr)return R;
    else if (R == nullptr)return L;
    Node* k = nullptr;
    L = splitLast(L, k);
    return concat(L, k, R);
}


//引用计数器加1
void add_ref(Node* t) {
    if (t != nullptr) {
        t->reference_counter++;
    }
}

//返回所有>=k的节点构成的树
Node* right(Node* t, KeyType k) {
    if (t == nullptr)return nullptr;
    if (t->key < k)return right(t->rc, k);
    Node* t2 = copy(t);
    add_ref(t->rc);
    //return concat(right(t->lc, k), t2, t->rc);
    t2->lc = right(t->lc, k);
   
    return t2;
}
//返回所有<=k的节点构成的树
Node* left(Node* t, KeyType k) {
    if (t == nullptr)return nullptr;
    if (t->key > k)return left(t->lc, k);
    Node* t2 = copy(t);
    add_ref(t->lc);
    //return concat(t->lc, t2, left(t->rc, k));
   
    t2->rc = left(t->rc, k);
    return t2;
}

//t:根节点;A[]:待插入的元组(已排序);m:元组个数
Node* multi_insert(Node* t, Tuple A[], int m, int (*rho)(int val1, int val2)) {
    //若p树为空，则构建p树
    if (t == nullptr) return build(A, m);
    if (m == 0) {
        add_ref(t);
        return t;
    }
    int b = binarySearch(A, m, t->key);//b相当于小于t->key的子数组长度
    int d = (b < m&& A[b].key == t->key) ? 1 : 0;//t->key在A[]中
    Node* L = multi_insert(t->lc, A, b, rho);
    Node* R = multi_insert(t->rc, A + b + d, m - b - d, rho);
    Node* t2 = copy(t);
    if (d == 1)t2->entry->val = rho(t->entry->val, A[b].val);
    return concat(L, t2, R);
}

Node* range(Node* t, KeyType kl, KeyType kr) {
    Node* cur = t;
    while (cur->key<kl || cur->key>kr) {
        if (cur->key < kl)cur = cur->rc;
        if (cur->key > kr)cur = cur->lc;
    }
    Node* L = right(cur->lc, kl);
    Node* R = left(cur->rc, kr);
    Node* mid = copy(cur);
    
    //return concat(L, mid, R);
    mid->lc = L;
    mid->rc = R;
    return mid;
}

Node* filter(Node* t, bool (*function)(Node* t)) {
    if (t == nullptr)return nullptr;
    Node* L = filter(t->lc, function);
    Node* R = filter(t->rc, function);
    if (function(t)) {
        return concat(L, copy(t), R);
    }
    else {
        return concat(L, R);
    }
}

void foreach_index(Node* t, void (*function)(Tuple* e, int i), int s) {
    if (t == nullptr)return;
    int left = size(t->lc);
    foreach_index(t->lc, function, s);
    foreach_index(t->rc, function, s + left + 1);
    function(t->entry, left);
}

//先序遍历p-树
void travel(Node* t) {
    if (t == nullptr) {
        cout << "# ";
        return;
    }

    cout << "[" << t->key << ":" << t->height << ":" << t->hash << "] ";
    travel(t->lc);
    travel(t->rc);
}

//done
void test_build() {
    int const m = 5;
    Tuple tuples[m];
    cout << "build p-tree" << endl;
    KeyType build_key[] = { 1,2,3,4,5 };
    ValueType build_value[] = { 101,102,103,104,105 };

    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];
        // cout << "[" << tuples[i].key << ":" << tuples[i].val << "] ";
    }
    // cout << endl;

    Node* indexRoot = nullptr;
    indexRoot = multi_insert(indexRoot, tuples, m, rho);

    cout << "pre order trvel" << endl;
    travel(indexRoot);

}

//done
void test_multi_insert() {
    Node* indexRoot = nullptr;

    //build----------------------------------------------------
    cout << "build p-tree" << endl;
    int const m = 10;
    Tuple tuples[m];
    KeyType build_key[] = { 1,3,4,5,9,11,13,15,17,19 };
    ValueType build_value[] = { 0,0,0,0,0,0,0,0,0,0 };
    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];
        // cout << "[" << tuples[i].key << ":" << tuples[i].val << "] ";
    }
    //cout << endl;
    indexRoot = multi_insert(indexRoot, tuples, m, rho);
    travel(indexRoot); cout << endl;
    //inset----------------------------------------------------
    int const m2 = 3;
    Tuple tuples2[m2];
    cout << "insert p-tree" << endl;
    KeyType insert_key[] = { 6,7,8 };
    ValueType insert_value[] = { 1,1,1 };
    for (int i = 0; i < m2; i++) {
        tuples2[i].key = insert_key[i];
        tuples2[i].val = insert_value[i];
        // cout << "[" << tuples[i].key << ":" << tuples[i].val << "] ";
    }
    //cout << endl;
    Node* indexRoot2 = multi_insert(indexRoot, tuples2, m2, rho);
    travel(indexRoot2); cout << endl;
    travel(indexRoot); cout << endl;
}

//done
void test_size() {
    int const m = 8;
    Tuple tuples[m];
    cout << "build p-tree" << endl;
    KeyType build_key[] = { 1,2,3,4,5,8,6,7 };
    ValueType build_value[] = { 101,102,103,104,105,108,106,107 };

    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];
        cout << "[" << tuples[i].key << ":" << tuples[i].val << "] ";
    }
    cout << endl;

    Node* indexRoot = nullptr;
    indexRoot = multi_insert(indexRoot, tuples, m, rho);

    cout << " size " << size(indexRoot) << endl;

}

//done
void test_binarySearch() {

    int A[] = { 1,2,4,5,6,7,8 ,9,11 };
    for (int i = 0; i <= 12; i++) {
        cout << i << ":" << binarySearch(A, 9, i) << " ";
    }
    cout << endl;
}

//done
void test_height(int const m) {
    Tuple* tuples = new Tuple[m];
    // cout << "build p-tree" << endl;
    for (int i = 0; i < m; i++) {
        tuples[i].key = i;
        tuples[i].val = i + 100;
        // cout << "[" << tuples[i].key << ":" << tuples[i].val << "] ";
    }
    cout << endl;
    Node* indexRoot = nullptr;
    indexRoot = multi_insert(indexRoot, tuples, m, rho);
    cout << m << ":" << getHeight(indexRoot) << endl;
    delete[]tuples;
}

//done
void test_concat_without_pivot() {
    int const m1 = 10, m2 = 3;
    Tuple tuples1[m1], tuples2[m2];
    cout << "build p-tree" << endl;

    KeyType build_key1[] = { 1,2,3,4,5,6,7,8,9,10, };
    ValueType build_value1[] = { 1,1,1,1,1,1,1,1,1,1 };
    for (int i = 0; i < m1; i++) {
        tuples1[i].key = build_key1[i];
        tuples1[i].val = build_value1[i];     
    }

    KeyType build_key2[] = { 17,18,19 };
    ValueType build_value2[] = { 3,3,3 };
    for (int i = 0; i < m2; i++) {
        tuples2[i].key = build_key2[i];
        tuples2[i].val = build_value2[i];     
    }
  
    Node* indexRoot1 = nullptr;
    Node* indexRoot2 = nullptr;
    Node* indexRoot3 = nullptr;

    indexRoot1 = multi_insert(indexRoot1, tuples1, m1, rho);
    indexRoot2 = multi_insert(indexRoot2, tuples2, m2, rho);
    travel(indexRoot1); cout << endl;
    travel(indexRoot2); cout << endl;
    indexRoot3 = concat(indexRoot1, indexRoot2);
    travel(indexRoot3); cout << endl;
    
}

//done
void test_concat() {
    int const m1 = 15, m2 = 3;
    Tuple tuples1[m1], tuples2[m2];
    cout << "build p-tree" << endl;

    KeyType build_key1[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    ValueType build_value1[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
    for (int i = 0; i < m1; i++) {
        tuples1[i].key = build_key1[i];
        tuples1[i].val = build_value1[i];
        // cout << "[" << tuples1[i].key << ":" << tuples1[i].val << "] ";
    }
    //cout << endl;

    KeyType build_key2[] = { 17,18,19 };
    ValueType build_value2[] = { 3,3,3 };
    for (int i = 0; i < m2; i++) {
        tuples2[i].key = build_key2[i];
        tuples2[i].val = build_value2[i];
        // cout << "[" << tuples2[i].key << ":" << tuples2[i].val << "] ";
    }
    // cout << endl;

    Node* indexRoot1 = nullptr;
    Node* indexRoot2 = nullptr;
    Node* indexRoot3 = new Node(new Tuple(16, 2), nullptr, nullptr);

    indexRoot1 = multi_insert(indexRoot1, tuples1, m1, rho);
    indexRoot2 = multi_insert(indexRoot2, tuples2, m2, rho);
    travel(indexRoot1); cout << endl;
    travel(indexRoot2); cout << endl;
    indexRoot3 = concat(indexRoot1, indexRoot3, indexRoot2);
    travel(indexRoot3);
    cout << endl;
}


//done
void tesy_left_right() {
    cout << "build p-tree" << endl;
    int const m = 18;
    Tuple tuples[m];
    KeyType build_key[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 };
    ValueType build_value[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];
    }
    Node* indexRoot = nullptr;
    indexRoot = multi_insert(indexRoot, tuples, m, rho); travel(indexRoot); cout << endl;
    Node* indexRoot2 = right(indexRoot->lc, 2); travel(indexRoot2); cout << endl;
    Node* indexRoot3 = left(indexRoot->rc, 14); travel(indexRoot3); cout << endl;

}

//done
void test_range() {
    cout << "build p-tree" << endl;
    int const m = 18;
    Tuple tuples[m];
    KeyType build_key[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 };
    ValueType build_value[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];
    }
    Node* indexRoot = nullptr;
    indexRoot = multi_insert(indexRoot, tuples, m, rho);
    travel(indexRoot);
    cout << endl;
    cout << "test_range " << endl;
    Node* indexRoot2 = range(indexRoot, 4, 12);
    //travel(indexRoot2);
    cout << endl;
    cout << endl;
    travel(indexRoot);
   
}

//done
void test_fileter() {
    cout << "build p-tree" << endl;
    int const m = 18;
    Tuple tuples[m];
    KeyType build_key[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 };
    ValueType build_value[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];
    }
    Node* indexRoot = nullptr;
    indexRoot = multi_insert(indexRoot, tuples, m, rho);
    travel(indexRoot); cout << endl;

    Node* indexRoot2 = filter(indexRoot,function);
    travel(indexRoot2); cout << endl;

}
//to do
void test_foreachindex() {

}

//done
void test_avl() {
    Node* a, * b, * c, * b1, * c1, * c2, * a2;
    c1 = new Node(new Tuple(3, 0), nullptr, nullptr);
    c2 = new Node(new Tuple(5, 0), nullptr, nullptr);
    c = new Node(new Tuple(4, 0), c1, c2);
    b1 = new Node(new Tuple(1, 0), nullptr, nullptr);
    b = new Node(new Tuple(2, 0), b1, c);
    a2 = new Node(new Tuple(7, 0), nullptr, nullptr);
    a = new Node(new Tuple(6, 0), b, a2);

    Node* root = a;
    updateHeight(root);
    travel(root); cout << endl;
    root = LR_rotation(root);
    travel(root); cout << endl;
}

//done 
void test_splitLast() {
    Node* indexRoot = nullptr;
    //build----------------------------------------------------
    cout << "build p-tree" << endl;
    int const m = 10;
    Tuple tuples[m];
    KeyType build_key[] = { 1,3,4,5,9,11,13,15,17,19};
    ValueType build_value[] = { 0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < m; i++) {
        tuples[i].key = build_key[i];
        tuples[i].val = build_value[i];    
    } 
    indexRoot = multi_insert(indexRoot, tuples, m, rho);
    travel(indexRoot); cout << endl;

    Node* k;
    Node* indexRoot2 = splitLast(indexRoot, k);
    travel(indexRoot2); cout << endl;
    Node* indexRoot3 = splitLast(indexRoot2, k);
    travel(indexRoot3); cout << endl;
}

int main()
{
    test_fileter();
    return 0;
}
