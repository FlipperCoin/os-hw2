#include <iostream>
#include <unistd.h>

int main() {
    long r = syscall(333);
    std::cout << "sys_hello returned " << r << std::endl;


    r = syscall(335); // see default value is 0.
    std::cout << "sys_get_weight returned " << r << std::endl;
    r = syscall(334,6); //set father to 6.
    std::cout << "sys_set_weight returned " << r << std::endl;
    r = syscall(334,-1); //check invalid arg.
    std::cout << "sys_set_weight returned " << r << std::endl;
    perror("Error");
    r = syscall(336); //check when no children.
    std::cout << "sys_get_leaf_children_sum returned " << r << std::endl;
    perror("Error");

    pid_t pid1 = fork();
    if(pid1!=0)
    {
        pid_t pid2 = fork();
        if(pid2!=0)
        {
            sleep(7);
            r = syscall(336);
            std::cout << "sys_get_leaf_children_sum A returned " << r << std::endl;
            r = syscall(337);
            std::cout << "sys_get_heaviest_ancestor A returned " << r << std::endl;
        }
        else
        {
            r = syscall(334,4);
            std::cout << "sys_set_weight returned C " << r << std::endl;
            r = syscall(337);
            std::cout << "sys_get_heaviest_ancestor C returned " << r << std::endl;
            sleep(10);
        }
    }
    else
    {
        r = syscall(334,5);
        std::cout << "sys_set_weight returned B " << r << std::endl;
        pid_t pid2 = fork();
        if(pid2!=0)
        {
            sleep(2);
            r = syscall(336);
            std::cout << "sys_get_leaf_children_sum B returned " << r << std::endl;

            r = syscall(337);
            std::cout << "sys_get_heaviest_ancestor B returned " << r << std::endl;
            sleep(10);
        }
        else
        {
            r = syscall(334,9);
            std::cout << "sys_set_weight returned D " << r << std::endl;

            r = syscall(337);
            std::cout << "sys_get_heaviest_ancestor D returned " << r << std::endl;
            sleep(10);
        }
    }
    
    return 0;
}