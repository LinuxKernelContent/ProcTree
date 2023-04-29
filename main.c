#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct TreeNode {
    int value;
    struct TreeNode** children;
    int num_children;
} TreeNode;

void createProcesses(TreeNode* node) {
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // In child process
        printf("Child process created: %d\n", node->value);
        for (int i = 0; i < node->num_children; i++) {
            createProcesses(node->children[i]);
        }
        exit(0);
    } else {
        // In parent process
        wait(NULL);
    }
}

int main() {
    // Create tree
    TreeNode* root = malloc(sizeof(TreeNode));
    root->value = 1;
    root->num_children = 3;
    root->children = malloc(sizeof(TreeNode*) * root->num_children);

    TreeNode* child1 = malloc(sizeof(TreeNode));
    child1->value = 2;
    child1->num_children = 2;
    child1->children = malloc(sizeof(TreeNode*) * child1->num_children);
    child1->children[0] = malloc(sizeof(TreeNode));
    child1->children[0]->value = 4;
    child1->children[0]->num_children = 0;
    child1->children[0]->children = NULL;
    child1->children[1] = malloc(sizeof(TreeNode));
    child1->children[1]->value = 5;
    child1->children[1]->num_children = 0;
    child1->children[1]->children = NULL;

    TreeNode* child2 = malloc(sizeof(TreeNode));
    child2->value = 3;
    child2->num_children = 1;
    child2->children = malloc(sizeof(TreeNode*) * child2->num_children);
    child2->children[0] = malloc(sizeof(TreeNode));
    child2->children[0]->value = 6;
    child2->children[0]->num_children = 0;
    child2->children[0]->children = NULL;

    root->children[0] = child1;
    root->children[1] = child2;
    root->children[2] = malloc(sizeof(TreeNode));
    root->children[2]->value = 7;
    root->children[2]->num_children = 0;
    root->children[2]->children = NULL;

    // Create processes
    createProcesses(root);

    // Wait for child processes to finish
    sleep(1);

    // View process tree
    system("pstree -p $PPID");

    return 0;
}
