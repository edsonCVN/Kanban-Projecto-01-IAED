#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* The maximum number of ID's and Tasks. */
#define Max_I_T 10000
/* The maximum number of characters for a Task descrition and maximum number of Users. */
#define Max_T_U 50
/* The maximum number of characters for a User and Ativity. */
#define Max_U_A 20
/* The maximum number of Ativities. */
#define Max_A 10

/* Texts */

#define error_t1 "too many tasks"
#define error_t2 "duplicate description"
#define error_t3 "invalid duration"
#define error_l "no such task"
#define error_n "invalid time"
#define error_u1 "user already exists"
#define error_u2 "too many users"
#define error_m1 "no such task"
#define error_m2 "task already started"
#define error_m3 "no such user"
#define error_m4 "no such activity"
#define error_d "no such activity"
#define error_a1 "duplicate activity"
#define error_a2 "invalid description"
#define error_a3 "too many activities"


typedef struct {

    char desc[Max_T_U+1];
    char user[Max_U_A+1];
    int id;
    int start_time;
    int prev_duration;

} Tasks;

typedef struct {

    char ativity[Max_U_A+1];
    Tasks task[Max_I_T];
    
} Kanban;

typedef struct {

    char desc[Max_T_U+1];
    int id;
    int start_time;
    
} Star_time;

Tasks task[Max_I_T];
Tasks empaty_task;

Kanban kan[Max_A];

Star_time shorter_time[Max_I_T];
Star_time temp_shorter_time;

int t = 0, n_tasks = 0, n_ativity = 3, n_users = 0, prev_duration_temp = 0, n_task_started = 0;
char temp_str[Max_T_U+1], desc_l[Max_I_T][Max_T_U+1], users[Max_T_U][Max_U_A+1];


/* funcoes*/
void comand_t(void);
int look_for_duplicate_t(void);

void comand_l(void);
void order_task_desc(int first,int last);
void print_task_list(void);

void comand_n(void);

void comand_u(void);
int user_retry(char user[Max_U_A+1]);

void comand_m(void);

int activity_existence( char ativ[Max_U_A+1]);
int id_current_activity(int id );
int ativ_ref(char ativ[Max_U_A+1]);

void comand_d(void);
void order_start_time(int first, int last);

void comand_a(void);

void str_reader(int sz) {
    int c, i=0;
    
    while (i <= sz && (c = getchar()) != EOF && c != '\n')
        temp_str[i++] = c;
    temp_str[i] = '\0';
    return;
}

int main() {
    
    char c;

    strcpy(kan[0].ativity, "TO DO\0");
    strcpy(kan[1].ativity, "IN PROGRESS\0");
    strcpy(kan[2].ativity, "DONE\0");

    while ((c = getchar ()) != 'q') {

        switch (c) {

            case 't':
                comand_t();
                break;
                
            case 'l':
                comand_l();
                break;
                
            case 'n':
                comand_n();
                break;
                
            case 'u':
                comand_u();
                break;
                
            case 'm':
                comand_m();
                break;
                
            case 'd':
                comand_d();
                break;
                
            case 'a':
                comand_a();
                break;

            default:
                break;
        }

    }

    return 0;
}

void comand_t(){
    
    if (n_tasks == Max_I_T){
        printf("%s\n", error_t1);
        return ;
    }

    scanf("%d ", &prev_duration_temp);
    
    str_reader(Max_T_U);

    if ( !look_for_duplicate_t()) {
        printf("%s\n", error_t2);
        return ;
    }
    
    else if ( prev_duration_temp < 0) {
        printf("%s\n", error_t3);
        return ;
    }
    
    task[n_tasks].prev_duration = prev_duration_temp;
    task[n_tasks].id = n_tasks+1;
    strcpy(task[n_tasks].desc, temp_str);
    strcpy(desc_l[n_tasks], temp_str);
    kan[0].task[n_tasks] = task[n_tasks];
    ++n_tasks;

    printf("task %d\n", n_tasks);
    return ;
}

int look_for_duplicate_t() {

    int i;

    for (i = 0; i < n_tasks+1; i++){
        if (strcmp(kan[0].task[i].desc, temp_str) == 0){
            return 0;
        }
    }
    return 1;
}

void comand_l(){
    int i, id;
    char c;
    
    
    if ((c = getchar()) == EOF || c == '\n') {
        order_task_desc(0, n_tasks-1);
        print_task_list();
        return;
    }
    
    for (i = 0; i < Max_I_T; i++) {
        if (scanf("%d", &id) == 1) {
            if (id > n_tasks || id <= 0) {
                printf("%d: %s\n", id, error_l);
            }
            else {
                for (i = 0; i < n_ativity; i++)
                    if (kan[i].task[id-1].id == id)
                        printf("%d %s #%d %s\n", id, kan[i].ativity,
                               task[id-1].prev_duration, task[id-1].desc);
            }
        }
        else
            break;
    }
}

void order_task_desc(int first,int last) {
    
    int i, j, pivot;
    char temp[Max_T_U+1];
    
    if (n_tasks == 1) {
        return;
    }

    if(first < last){
        pivot = first;
        i = first;
        j = last;

        while(i < j){
            while(strcmp(desc_l[i], desc_l[pivot]) <= 0 && i < last)
                i++;
            while(strcmp(desc_l[j], desc_l[pivot]) > 0)
                j--;
            if(i < j){
                strcpy(temp, desc_l[i]);
                strcpy(desc_l[i], desc_l[j]);
                strcpy(desc_l[j], temp);
             }
        }

        if (pivot != j) {
            strcpy(temp, desc_l[pivot]);
            strcpy(desc_l[pivot], desc_l[j]);
            strcpy(desc_l[j], temp);
        }
        order_task_desc(first,j-1);
        order_task_desc(j+1,last);

    }
    return;
}

void print_task_list() {
    
    int i, j, k, done = 0;
    
    for (i = 0; i < n_tasks; i++){
        done = 0;
        for (j = 0; j < n_ativity; j++) {
            for (k = 0; k < n_tasks; k++){
                if ( !strcmp(kan[j].task[k].desc, desc_l[i]) ){
                    printf("%d %s #%d %s\n", kan[j].task[k].id, kan[j].ativity,
                           kan[j].task[k].prev_duration, desc_l[i]);
                    done = 1;
                    break;
                }
            }
            if (done)
                break;
        }
    }
    return;
}

void comand_n() {
    
    int temp;
    
    if ((scanf("%d", &temp)) != 1 || temp < 0)
        printf("%s\n", error_n);
    else {
        t += temp;
        printf("%d\n", t);
    }
    return;
}

void comand_u() {
    
    int i;
    char c, user[Max_U_A+1];
    
    if ((c = getchar()) == EOF || c == '\n') {
        for (i = 0; i < n_users; i++)
            printf("%s\n", users[i]);
    return;
    }
    
    else {
        scanf(" %s", user);
        if (!user_retry(user)) {
            printf("%s\n", error_u1);
            return;
        }
        
        else if (n_users == Max_T_U) {
            printf("%s\n", error_u2);
            return;
        }
        else {
            strcpy(users[n_users], user);
            ++n_users;
        }
            
    }
}

int user_retry(char user[Max_U_A+1]) {
    
    int i;
    
    for (i = 0; i < n_users; i++) {
        if (!strcmp(user, users[i]))
            return 0;
    }
    return 1;
}

void comand_m() {
    
    int id, temp = 0, ativ_re = 0, duration = 0;
    char user[Max_U_A+1];
    
    scanf("%d %s " , &id, user);

    str_reader(Max_U_A);
        
    if (id > n_tasks) {
        printf("%s\n", error_m1);
        return ;
    }
    
    else if (!strcmp(temp_str, "TO DO")) {
        printf("%s\n", error_m2);
        return ;
    }
    
    else if (user_retry(user)) {
        printf("%s\n", error_m3);
        return ;
    }
    
    else if (!activity_existence(temp_str)) {
        printf("%s\n", error_m4);
        return ;
    }
    
    temp = id_current_activity(id);
    
    if (strcmp(temp_str, "DONE\0") == 0){
        kan[2].task[id-1] = kan[temp].task[id-1];
        if (kan[temp].task[id-1].start_time == 0)
            duration = 0;
        else
            duration = t - kan[temp].task[id-1].start_time;
        printf("duration=%d slack=%d\n", duration, duration - kan[temp].task[id-1].prev_duration);
        kan[temp].task[id-1] = empaty_task;
        return;
    }
    else {
        ativ_re = ativ_ref(temp_str);
        kan[ativ_re].task[id-1] = kan[temp].task[id-1];
        if (!temp) {
            kan[ativ_re].task[id-1].start_time = t;
            shorter_time[n_task_started].id = kan[ativ_re].task[id-1].id;
            shorter_time[n_task_started].start_time = kan[ativ_re].task[id-1].start_time;
            strcpy(shorter_time[n_task_started].desc, kan[ativ_re].task[id-1].desc);
            ++n_task_started;
        }
        kan[temp].task[id-1] = empaty_task;
    }
}

int activity_existence(char ativ[Max_U_A+1]) {
    
    int i;
    for (i = 0; i < n_ativity; i++)
        if (strcmp(kan[i].ativity, ativ) == 0)
            return 1;
    return 0;
}

int id_current_activity(int id ) {
    
    int i;
    
    for (i = 0; i < n_ativity; i++){
        if (kan[i].task[id-1].id == id)
            return i;
    }
    return 5;
}

int ativ_ref(char ativ[Max_U_A+1]) {
    
    int i;
    
    for (i = 0; i < n_ativity; i++){
        if (strcmp(kan[i].ativity, ativ) == 0)
            return i;
    }
    return Max_A + 1;
}

void comand_d() {
    int i = 0, k, ativ_re;
    char c, ativity[Max_U_A+1];
    
    getchar();
    
    while (i <= Max_U_A && (c = getchar()) != EOF && c != '\n')
        ativity[i++] = c;
    ativity[i] = '\0';
    
    if ((ativ_re = ativ_ref(ativity)) == Max_A + 1) {
        printf("%s\n", error_d);
        return;
    }
    
    else if (strcmp("TO DO\0", ativity)==0) {
        for (i = 0; i < n_tasks; i++){
            for (k = 0; k < n_tasks; k++){
                if (!strcmp(kan[0].task[k].desc,desc_l[i]) ){
                    printf("%d %d %s\n", kan[0].task[k].id,
                           kan[0].task[k].start_time, kan[0].task[k].desc);
                    break;
                }
            }
        }
        return;
    }
    
    order_task_desc(0, n_tasks-1);
    order_start_time(0, n_tasks-1);
    
    for (i = 0; i < n_tasks; i++){
        for (k = 0; k < n_tasks; k++){
            if (kan[ativ_re].task[k].id == shorter_time[i].id){
                printf("%d %d %s\n", kan[ativ_re].task[k].id,
                       kan[ativ_re].task[k].start_time, kan[ativ_re].task[k].desc);
                break;
            }
        }
    }
    return;
}

void order_start_time(int first, int last) {
    
    int i, j, pivot, temp;

    if(first < last){
        pivot = first;
        i = first;
        j = last;

        while(i < j){
            while(shorter_time[i].id<=shorter_time[pivot].id && i<last)
                i++;
            while(shorter_time[j].id>shorter_time[pivot].id)
                j--;
            if(i < j){
                temp = shorter_time[i].id;
                shorter_time[i].id = shorter_time[j].id;
                shorter_time[j].id = temp;
            }
        }

        temp = shorter_time[pivot].id;
        shorter_time[pivot].id = shorter_time[j].id;
        shorter_time[j].id = temp;
        order_start_time(first,j-1);
        order_start_time(j+1,last);
       }
    return;
}

void comand_a() {
    
    int i = 0, lower = 0;
    char c, ativity[Max_U_A+1];
    
    if ((c = getchar()) == EOF || c == '\n') {
        for (i = 0; i < n_ativity; i++){
            printf("%s\n", kan[i].ativity);
        }
        return;
    }
    
    while (i <= Max_U_A && (c = getchar()) != EOF && c != '\n'){
        ativity[i++] = c;
        if (islower(c))
            lower = 1;
    }
    ativity[i] = '\0';
    
    if (ativ_ref(ativity) != Max_A + 1)
        printf("%s\n", error_a1);
    
    else if (lower)
        printf("%s\n", error_a2);
    
    else if (n_ativity == Max_A)
        printf("%s\n", error_a3);
    
    else {
        strcpy(kan[n_ativity].ativity, ativity);
        ++n_ativity;
    }
    return;
}
