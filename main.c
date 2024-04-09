#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct car {
    char model[50];
    int power;
    enum {ICE, ELECTRIC, HYBRID} engine_type;
    union {
        float fuel_cons;
        int range;
    } durability;
    float acceleration0_100;
    struct car *next;
};


const char* engine_type_to_str(struct car *object)
{
    switch (object -> engine_type)
    {
        case ICE: return "ICE";
        case ELECTRIC: return "Electric";
        case HYBRID: return "Hybrid";
    }
}

int str_to_engine_type(const char* s) {
    if (strcmp(s, "ICE") == 0) return 0;
    else if (strcmp(s, "Electric") == 0) return 1;
    else if (strcmp(s, "Hybrid") == 0) return 2;
    else return -1;
}

struct car *create_list() {
    // Выделение памяти под корень списка
    struct car *tmp = malloc(sizeof(struct car));
    // Присваивание значения узлу
    for (int i = 0; i < 4; i++) tmp -> model[i] = "head"[i];
    // Присваивание указателю на следующий элемент значения NULL
    tmp -> next = NULL;
    return(tmp);
}

void add(struct car *head, const char *s, int power, float acceleration, int engine_type, float durability) {
    struct car *tail;
    tail = head;
    while (tail -> next != NULL) {
       tail = tail -> next;
    }
    struct car *tmp = malloc(sizeof(struct car));
    int i = 0;
    while (s[i] != '\0' && s[i] != '\n') {
        tmp -> model[i] = s[i];
        i++;
    }
    tmp -> power = power;
    tmp -> acceleration0_100 = acceleration;
    tmp -> engine_type = engine_type;
    if (tmp -> engine_type == ICE || tmp -> engine_type == HYBRID) tmp -> durability.fuel_cons = durability;
    else tmp -> durability.range = durability;
    tmp -> next = NULL;
    tail -> next = tmp;
}

void delete(struct car *head, const char *model) {
    struct car * cur_car;
    struct car * prev_car;
    cur_car = head;
    int count = 0;
    while (cur_car -> next != NULL) {
        prev_car = cur_car;
        cur_car = cur_car -> next;
        if (strstr(cur_car -> model, model)) {

                prev_car -> next = cur_car -> next;
//                cur_car = (struct car*) calloc(1, sizeof(struct car));
                free(cur_car);
                count += 1;
                cur_car = prev_car;
            }
    }
    printf("%d objects was deleted\n", count);
}


void input(const char *filename, struct car *head) {
    FILE *fin;
    fin = fopen(filename, "r");
    if (!fin) {
        printf("Data base uploading error\n");
        return;
    }
    char input_string[50];
    while (fgets(input_string, 50, fin) != NULL) {
        char model[50];
        int i = 0;

        int im = 0;
        while (input_string[i] != ':') {
            model[im] = input_string[i];
            i++;
            im++;
        }
        model[im] = '\0';
        i++;

        int ip = 0;
        char power_str[50];
        while (input_string[i] != ':') {
            power_str[ip] = input_string[i];
            i++;
            ip++;
        }
        power_str[ip] = '\0';
        int power = strtol(power_str, NULL, 10);
        i++;

        int ia = 0;
        char acc_str[50];
        while (input_string[i] != ':') {
            acc_str[ia] = input_string[i];
            i++;
            ia++;
        }
        acc_str[ia] = '\0';
        i++;
        float acceleration = atof(acc_str);

        int ie = 0;
        char str_engine_type[50];
        while (input_string[i] != ':') {
            str_engine_type[ie] = input_string[i];
            i++;
            ie++;
        }
        power_str[ip] = '\0';
        i++;
        int engine_type = strtol(str_engine_type, NULL, 10);


        int id = 0;
        char str_durability[50];
        while (input_string[i] != '\0' && input_string[i] != '\n') {
            str_durability[id] = input_string[i];
            i++;
            id++;
        }
        str_durability[id] = '\0';
        i++;
        float durability = atof(str_durability);

        add(head, model, power, acceleration, engine_type, durability);
    }
    printf("Data base uploaded successfully\n");
}

void output(struct car * elem) {
    printf("Car model: %s\n"
           "Power: %d hp\n"
           "0-100 acceleration: %.1f s\n" , elem -> model, elem -> power, elem -> acceleration0_100);
    printf("Engine type: %s\n",  engine_type_to_str(elem));
    if (elem -> engine_type == ICE || elem -> engine_type == HYBRID) {
        printf("Fuel consumption: %.1f L/100km\n",  elem -> durability.fuel_cons);
    }
    else printf("Range: %d km\n",  elem -> durability.range);
    printf("------------------\n");
}

void search(struct car * head, const char *request){
    struct car *cur_car;
    cur_car = head;
    int flag = 0;
    printf("Found objects:\n");
    printf("------------------\n");
    while (cur_car -> next != NULL) {
        cur_car = cur_car -> next;
        if (strstr(cur_car -> model, request)) {
            flag = 1;
            output(cur_car);
        }
    }
    if (flag == 0) printf("-");
}


void db_output(struct car * head) {
    struct car *cur_car = head;
    if (cur_car -> next != NULL) cur_car = cur_car -> next;
    else {
        printf("-\n");
        return;
    }
    printf("------------------\n");
    while (cur_car != NULL) {
        output(cur_car);
        cur_car = (struct car *) cur_car -> next;
    }
}
void print_command_list() {
    printf("q/quit/exit - exit\n");
    printf("input - upload data base\n");
    printf("output - print entire data base\n");
    printf("search - search by model\n");
    printf("add - add car to database\n");
    printf("delete - delete car from database\n");

}

int main() {
    struct car *head = create_list();
    char command[50];
    char filename[50];
    char request[50];
    char model[50];
    char model_ending[50];
    int power;
    float acceleration;
    char str_engine_type[10];
    float durability;

    while (1){
        printf("\nEnter command (help - command list):\n");

        scanf("%s", command);
        if (strcmp(command,"help") == 0) {
            print_command_list();
        }
        else if (strcmp(command,"output") == 0) {
            printf("\nData base output:\n");
            db_output(head);
        }
        else if (strcmp(command, "input") == 0) {
            printf("Enter name of data base to upload: \n");
            scanf("%s", filename);
            input(filename, head);
        }
        else if (strcmp(command,"search") == 0) {
            printf("Enter search request: \n");
            scanf("%s", request);
            search(head, request);
        }
        else if (strcmp(command,"add") == 0) {
            printf("Enter car model: \n");
            //char* model1;
            scanf("%s", model);
            fgets(model_ending, 50, stdin);
            //model1 = strcat(model, " ");
            printf("Enter car power (hp): \n");
            scanf("%d", &power);
            printf("Enter 0-100 acceleration time (s): \n");
            scanf("%f", &acceleration);
            str_engine_type[0] = '\0';
            while (str_to_engine_type(str_engine_type) == -1) {
                printf("Enter car engine type: (ICE/Electric/Hybrid)\n");
                scanf("%s", str_engine_type);
            }
            if (str_to_engine_type(str_engine_type) == ICE || str_to_engine_type(str_engine_type) == HYBRID) {
                printf("Enter average fuel consumption value (L/100km): \n");
            }
            else printf("Enter estimated range value (km): \n");
            scanf("%f", &durability);
            add(head, strcat(model, model_ending), power, acceleration, str_to_engine_type(str_engine_type), durability);
            printf("Car added successfully\n");
        }
        else if (strcmp(command,"delete") == 0) {
            printf("Enter model of car to delete: \n");
            scanf("%s", model);
            delete(head, model);
        }
        else if (strcmp(command,"q") == 0 || strcmp(command,"exit") == 0 || strcmp(command,"quit") == 0) {
            return 0;
        }
        else if (strcmp(command, "f") == 0) input("cars.txt", head);
        else printf("Unknown command\n");
        }
    }
