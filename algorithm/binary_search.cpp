//
// Created by SMS on 2022/4/18.
//


//>=target的最左元素
int search1(int target,vector<int> nums){
    int left=0,right=nums.size()-1;
    while (left<=right){
        int mid=(left+right)>>1;
        if(nums[mid] >= target){
            right=mid-1;
        }else{
            left=mid+1;
        }
    }
    return left;
}

//>target的最左元素
int search2(int target,vector<int> nums){
    int left=0,right=nums.size()-1;
    while (left<=right){
        int mid=(left+right)>>1;
        if(nums[mid] > target){
            right=mid-1;
        }else{
            left=mid+1;
        }
    }
    return left;
}

//<=target的最右元素
int search3(int target,vector<int> nums){
    int left=0,right=nums.size()-1;
    while (left<=right){
        int mid=(left+right)>>1;
        if(nums[mid] <= target){
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    return right;
}

//<target的最右元素
int search4(int target,vector<int> nums){
    int left=0,right=nums.size()-1;
    while (left<=right){
        int mid=(left+right)>>1;
        if(nums[mid] < target){
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    return right;
}
