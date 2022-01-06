#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <fin_core.h>
#include <peano.h>


struct ap {
    fin_instance_t *a;
    fin_instance_t *b;
};

void ap_handler(fin_instance_t *a, fin_instance_t *b, void *data) {
    char *x = a->m_declaration->m_name;
    char *y = b->m_declaration->m_name;
    fprintf(stdout, "CB active pair %s(%p)-%s(%p)\n", x, &a->m_connections[0], y, &b->m_connections[0]);
    if (data == NULL) return;
    ((struct ap*) data)->a = a;
    ((struct ap*) data)->b = b;
}

int main() {
    fin_configuration_t *the_configuration = peano_add_2_1();
    /*reduce*/
    reduce(the_configuration);

    //free the net
    free_net(the_configuration->m_net);
    //free the configuration
    free_configuration(the_configuration);

    the_configuration = peano_number(10);
    to_dot_net(stdout, the_configuration->m_net);

    return 0;
}

