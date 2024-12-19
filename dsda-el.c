#include<stdio.h>
#include<stdlib.h>
typedef struct User_Data
{
    int following[10000];
}U;

typedef struct STACK
{
    int uid, tid;
}S;


typedef struct {
    U user[10000];
    int top;
    S *stack;   
} Twitter;


void PRINT_FEED(int* arr,int *retSize)
{
    
    for(int i =0; i<(*retSize); i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

Twitter* twitterCreate() {
    Twitter* object = (Twitter *)malloc(sizeof(Twitter));
    object->top = -1; 
    object->stack = (S *)calloc(10000,sizeof(S)); 
    return object;
}

void twitterPostTweet(Twitter* obj, int userId, int tweetId) {
    obj->stack[++(obj->top)].tid = tweetId;
    obj->stack[(obj->top)].uid = userId;
}



int* twitterGetNewsFeed(Twitter* obj, int userId, int* retSize) {
    int a = obj->top; 
    int i = 0;
    *retSize = 0;     
    int* head = NULL; 

    while (i < 10 && a >= 0) { 
        int tweetUserId = obj->stack[a].uid; 
        if (tweetUserId == userId || obj->user[userId].following[tweetUserId] == 1) {
            

            int* newHead = NULL;

            
            if (head == NULL) {
                newHead = (int*)malloc(sizeof(int));
            } else {
                newHead = (int*)realloc(head, (i + 1) * sizeof(int));
            }

            if (newHead == NULL) {
                
                free(head);
                *retSize = 0;
                return NULL;
            }

            head = newHead;
            head[i++] = obj->stack[a].tid; 
        }
        a--;
    }

    *retSize = i; 
    return head;
}


void twitterFollow(Twitter* obj, int followerId, int followeeId) {
    obj->user[followerId].following[followeeId] = 1;
}

void twitterUnfollow(Twitter* obj, int followerId, int followeeId) {
    obj->user[followerId].following[followeeId] = 0;
}

void twitterFree(Twitter* obj) {
    free(obj->stack);
    free(obj);
    
}
int main()
{
    int choice;
    Twitter* obj = twitterCreate();
    while (1)
    {
        printf("\nenter 1 to post a tweet, \nenter 2 to follow an user \nenter 3 to get feed \nenter 4 to unfollow an user \nenter 5 to exit the platform");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            {
                int a,b;
                printf("\nenter user id and tweet id to post a tweet\n");
                scanf("%d %d",&a,&b);
                twitterPostTweet(obj, a, b);
            };
            break;
        case 2:
            {
                int a,b;
                printf("\nenter follower id and followee id to post a tweet\n");
                scanf("%d %d",&a,&b);
                twitterFollow(obj, a, b);
            };break;
        case 3:
            {
                int *retSize,*param_2,a;
                printf("\n\n\nenter user id to get feed\n");
                scanf("%d", &a);
                param_2 = twitterGetNewsFeed(obj, a, retSize);
                printf("\nUser %d feed -> ", a);
                PRINT_FEED(param_2,retSize);

            };break;
        case 4:
            {
                int followerId,followeeId;
                printf("\nEnter the followerId and followeeId\n");
                scanf("%d %d",&followerId,&followeeId);
                twitterUnfollow(obj, followerId, followeeId);
            };break;
        case 5:
            {twitterFree(obj);
            return 0;};
            break;
        
        default:
            break;
        }
    }
    
return 0;  
}
/**
 * Your Twitter struct will be instantiated and called as such:
 * Twitter* obj = twitterCreate();
 * twitterPostTweet(obj, userId, tweetId);
 
 * int* param_2 = twitterGetNewsFeed(obj, userId, retSize);
 
 * twitterFollow(obj, followerId, followeeId);
 
 * twitterUnfollow(obj, followerId, followeeId);
 
 * twitterFree(obj);
*/