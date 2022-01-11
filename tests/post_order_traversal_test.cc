#include <gtest/gtest.h>

namespace explore_post_order_traversal {

typedef struct node_s {
    struct node_s *left_p;
    struct node_s *right_p;
    int data;
} node_t;

#define STACK_SIZE 100

typedef enum stack_el_enum { node_element, right_element } stack_el_type;

typedef struct stack_el_s {
    node_s *ptr = NULL;
    stack_el_type type;
} stack_el_t;

typedef struct stack_s {
    stack_el_t *ptr;
    int top = -1;
} stack_t;

stack_s *create(size_t size) {
    stack_t *stack_p = (stack_s *)malloc(sizeof(stack_t));
    stack_p->top = -1;
    stack_p->ptr = (stack_el_t *)calloc(size, sizeof(stack_el_t));
    return stack_p;
}

void destroy(stack_s *stack_p) {
    free(stack_p->ptr);
    free(stack_p);
}

void push(stack_s *stack_p, node_t *ptr, stack_el_type type) {
    ++stack_p->top;
    stack_p->ptr[stack_p->top].ptr = ptr;
    stack_p->ptr[stack_p->top].type = type;
}

void pop(stack_s *stack_p, node_t **ptr, stack_el_type *type) {
    *ptr = stack_p->ptr[stack_p->top].ptr;
    *type = stack_p->ptr[stack_p->top].type;
    --stack_p->top;
}

int is_empty(stack_s *stack_p) { return stack_p->top == -1; }

TEST(PostOrderTraversal, StackPushPop) {
    node_t node_1 = {.data = 1, .left_p = NULL, .right_p = NULL};
    stack_s *stack_p = create(STACK_SIZE);
    EXPECT_TRUE(is_empty(stack_p));
    push(stack_p, &node_1, node_element);
    push(stack_p, node_1.right_p, right_element);
    node_t *ptr;
    stack_el_type type;
    pop(stack_p, &ptr, &type);
    EXPECT_EQ(ptr, node_1.right_p);
    EXPECT_EQ(type, right_element);
    EXPECT_TRUE(!is_empty(stack_p));
    pop(stack_p, &ptr, &type);
    EXPECT_EQ(ptr, &node_1);
    EXPECT_EQ(type, node_element);
    EXPECT_TRUE(is_empty(stack_p));
    destroy(stack_p);
}

int walk_tree(node_t *tree, int *result, int max_size) {
    if (NULL == tree) {
        return 0;
    }
    int node_count = 0;
    node_t *w_p = tree;
    stack_s *stack_p = create(STACK_SIZE);
    do {
        while (NULL != w_p) {
            push(stack_p, w_p, node_element);
            if (NULL != w_p->right_p)
                push(stack_p, w_p->right_p, right_element);
            w_p = w_p->left_p;
        }
        while (!is_empty(stack_p)) {
            stack_el_type type;
            pop(stack_p, &w_p, &type);
            if (node_element == type) {
                *(result + node_count++) = w_p->data;
            } else {
                break;
            }
        }
    } while (!is_empty(stack_p));
    destroy(stack_p);
    return node_count;
}

TEST(PostOrderTraversal, EmptyTree) {
    int result[STACK_SIZE];
    int size = walk_tree(NULL, result, STACK_SIZE);
    EXPECT_EQ(size, 0);
}

TEST(PostOrderTraversal, OneNode) {
    node_t node_1 = {.data = 1, .left_p = NULL, .right_p = NULL};
    int result[STACK_SIZE];
    int size = walk_tree(&node_1, result, STACK_SIZE);
    EXPECT_EQ(size, 1);
    EXPECT_EQ(result[0], 1);
}

TEST(PostOrderTraversal, LeftNode) {
    //   1
    // 2
    node_t node_2 = {.data = 2, .left_p = NULL, .right_p = NULL};
    node_t node_1 = {.data = 1, .left_p = &node_2, .right_p = NULL};
    int result[STACK_SIZE];
    int size = walk_tree(&node_1, result, STACK_SIZE);
    EXPECT_EQ(size, 2);
    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 1);
}

TEST(PostOrderTraversal, RightNode) {
    //  1
    //    2
    node_t node_2 = {.data = 2, .left_p = NULL, .right_p = NULL};
    node_t node_1 = {.data = 1, .left_p = NULL, .right_p = &node_2};
    int result[STACK_SIZE];
    int size = walk_tree(&node_1, result, STACK_SIZE);
    EXPECT_EQ(size, 2);
    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 1);
}

TEST(PostOrderTraversal, LeftRightNode) {
    //   1
    // 2   3
    node_t node_2 = {.data = 2, .left_p = NULL, .right_p = NULL};
    node_t node_3 = {.data = 3, .left_p = NULL, .right_p = NULL};
    node_t node_1 = {.data = 1, .left_p = &node_2, .right_p = &node_3};
    int result[STACK_SIZE];
    int size = walk_tree(&node_1, result, STACK_SIZE);
    EXPECT_EQ(size, 3);
    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 3);
    EXPECT_EQ(result[2], 1);
}

TEST(PostOrderTraversal, TreeOneSeven) {
    //      1
    //   2      5
    // 3   4  6   7
    node_t node_7 = {.data = 7, .left_p = NULL, .right_p = NULL};
    node_t node_6 = {.data = 6, .left_p = NULL, .right_p = NULL};
    node_t node_5 = {.data = 5, .left_p = &node_6, .right_p = &node_7};
    node_t node_3 = {.data = 3, .left_p = NULL, .right_p = NULL};
    node_t node_4 = {.data = 4, .left_p = NULL, .right_p = NULL};
    node_t node_2 = {.data = 2, .left_p = &node_3, .right_p = &node_4};
    node_t node_1 = {.data = 1, .left_p = &node_2, .right_p = &node_5};
    int result[STACK_SIZE];
    int size = walk_tree(&node_1, result, STACK_SIZE);
    EXPECT_EQ(size, 7);
    EXPECT_EQ(result[0], 3);
    EXPECT_EQ(result[1], 4);
    EXPECT_EQ(result[2], 2);
    EXPECT_EQ(result[3], 6);
    EXPECT_EQ(result[4], 7);
    EXPECT_EQ(result[5], 5);
    EXPECT_EQ(result[6], 1);
}

// #define N 1
// #define Z 0
// // Iterative Post-Order traversal (Left-Right-Root)
// void tree_walk(node_s *node_p)
// {
//     node_s* w_p = node_p;
//     stack_t stack = create_stack(); // .push(void* node, int side) .pop()
//     push(stack, w_p, Z);     // ? move into the first nested loop
//     while(!is_empty(stack)){ // ? change to do .. while
//         while(w_p != NULL) {
//             if(w_p->right_p != NULL) push(stack, w_p->right_p, N);
//             push(stack, w_p, Z);
//             w_p = w_p->left_p;
//         }
//         while (1) {          // ? should be !is_empty(stack)
//             int flag = pop(stack, &w_p);
//             if(flag == Z){
//                 printf("%d ", w_p->data);
//             } esle {
//                 break;
//             }
//         }
//     }
// }

}; // namespace explore_post_order_traversal
