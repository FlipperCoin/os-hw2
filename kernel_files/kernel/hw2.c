#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/current.h>

asmlinkage long sys_hello(void) {
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_weight(int weight) {
    if(weight <0)
    {
        return -EINVAL;
    }
    current->weight = weight;
    return 0;
}

asmlinkage long sys_get_weight(void) {
    return current->weight;
}

int get_leaf_children_sum(struct task_struct* curr)
{
    int sum = 0;
    struct task_struct* task;
    struct list_head* list;
    if(list_empty(&curr->children))
    {
        return curr->weight;
    }

    list_for_each(list, &curr->children) {
        task = list_entry(list, struct task_struct, sibling);
        sum += get_leaf_children_sum(task);
    }
    return sum;
}

asmlinkage long sys_get_leaf_children_sum(void) {
    
    if(list_empty(&current->children))
    {
        return -ECHILD;
    }
    return get_leaf_children_sum(current);
}

asmlinkage long sys_get_heaviest_ancestor(void) {
    
    int max = current->weight;
    pid_t max_pid = current->pid;
    struct task_struct* curr = current;
    while(curr->real_parent != curr)
    {
        if(curr->real_parent->weight > max)
        {
            max = curr->real_parent->weight;
            max_pid = curr->real_parent->pid;
        }
        curr = curr->real_parent;
    }
    return max_pid;
}

