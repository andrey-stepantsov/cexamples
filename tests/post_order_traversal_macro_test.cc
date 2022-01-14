#include <gtest/gtest.h>

namespace explore_post_order_traversal2 {

typedef struct node_s {
    struct node_s *left_p;
    struct node_s *right_p;
    int data;
} node_t;

#define STACK_SIZE 100

typedef enum stack_el_enum { move_cmd, print_cmd } cmd_type;

typedef struct stack_el_s {
    node_s *ptr = NULL;
    cmd_type type;
} stack_el_t;

typedef struct stack_s {
    stack_el_t *ptr;
    int top = -1;
} stack_t;

stack_t *create(size_t size) {
    stack_t *stack_p = (stack_s *)malloc(sizeof(stack_t));
    stack_p->top = -1;
    stack_p->ptr = (stack_el_t *)calloc(size, sizeof(stack_el_t));
    return stack_p;
}

void destroy(stack_t *stack_p) {
    free(stack_p->ptr);
    free(stack_p);
}

#define _STACK_MACRO 1 // comment to try without macro support

#ifdef _STACK_MACRO
#define  push(stack_p, _ptr, _type) \
    ++stack_p->top; \
    stack_p->ptr[stack_p->top].ptr = _ptr; \
    stack_p->ptr[stack_p->top].type = _type;
#else
void push(stack_t *stack_p, node_t *ptr, cmd_type type) {
    ++stack_p->top;
    stack_p->ptr[stack_p->top].ptr = ptr;
    stack_p->ptr[stack_p->top].type = type;
}
#endif

#ifdef _STACK_MACRO
#define pop(stack_p, _ptr, _type) \
    *_ptr = stack_p->ptr[stack_p->top].ptr; \
    *_type = stack_p->ptr[stack_p->top].type; \
    --stack_p->top;
#else
void pop(stack_t *stack_p, node_t **ptr, cmd_type *type) {
    *ptr = stack_p->ptr[stack_p->top].ptr;
    *type = stack_p->ptr[stack_p->top].type;
    --stack_p->top;
}
#endif

#ifdef _STACK_MACRO
#define is_empty(stack_p) (stack_p->top == -1)
#elif
int is_empty(stack_t *stack_p) { return stack_p->top == -1; }
#endif

TEST(PostOrderTraversal, StackPushPop) {
    node_t node_1 = {.data = 1, .left_p = NULL, .right_p = NULL};
    stack_t *stack_p = create(STACK_SIZE);
    EXPECT_TRUE(is_empty(stack_p));
    push(stack_p, &node_1, print_cmd);
    push(stack_p, node_1.right_p, move_cmd);
    node_t *ptr;
    cmd_type type;
    pop(stack_p, &ptr, &type);
    EXPECT_EQ(ptr, node_1.right_p);
    EXPECT_EQ(type, move_cmd);
    EXPECT_TRUE(!is_empty(stack_p));
    pop(stack_p, &ptr, &type);
    EXPECT_EQ(ptr, &node_1);
    EXPECT_EQ(type, print_cmd);
    EXPECT_TRUE(is_empty(stack_p));
    destroy(stack_p);
}

int walk_tree(node_t *tree, int *result, int max_size) {
    if (NULL == tree) {
        return 0;
    }
    int node_count = 0;
    node_t *w_p = tree;
    stack_t *stack_p = create(max_size);
    push(stack_p, w_p, print_cmd);
    push(stack_p, w_p->right_p, move_cmd);
    push(stack_p, w_p->left_p, move_cmd);
    while (!is_empty(stack_p)) {
        cmd_type cmd;
        pop(stack_p, &w_p, &cmd);
        if (move_cmd == cmd) {
            if (NULL != w_p) {
                push(stack_p, w_p, print_cmd);
                push(stack_p, w_p->right_p, move_cmd);
                push(stack_p, w_p->left_p, move_cmd);
            }
            continue;
        }
        if (print_cmd == cmd) {
            *(result + node_count++) = w_p->data; // "print to the output"
            continue;
        }
    };
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

}; // namespace explore_post_order_traversal2
