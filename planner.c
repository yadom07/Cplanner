#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 1000
#define FILE_NAME "planner.txt"

typedef struct {
    char title[64];
    char date[16];   
    char time[16];   
    char cat[32];
    char desc[256];
    int done;
} Task;

static Task t[MAX];
static int n = 0;


void inputLine(const char *msg, char *buf, int size) {
    printf("%s", msg);

    if (!fgets(buf, size, stdin)) {
        buf[0] = 0;
        return;
    }

    if (buf[strlen(buf) - 1] != '\n') {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }

    buf[strcspn(buf, "\r\n")] = 0;
}

int isBlank(const char *s) {
    if (!s) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}


void saveAll() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) return;

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%d\n",
            t[i].title,
            t[i].date,
            t[i].time,
            t[i].cat,
            t[i].desc,
            t[i].done
        );
    }
    fclose(fp);
}

void loadAll() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return;

    n = 0;
    char line[700];

    while (n < MAX && fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;

        char *p = strtok(line, "|"); if (!p) continue;
        strncpy(t[n].title, p, sizeof(t[n].title)-1);
        t[n].title[sizeof(t[n].title)-1] = 0;

        p = strtok(NULL, "|"); if (!p) continue;
        strncpy(t[n].date, p, sizeof(t[n].date)-1);
        t[n].date[sizeof(t[n].date)-1] = 0;

        p = strtok(NULL, "|"); if (!p) continue;
        strncpy(t[n].time, p, sizeof(t[n].time)-1);
        t[n].time[sizeof(t[n].time)-1] = 0;

        p = strtok(NULL, "|"); if (!p) continue;
        strncpy(t[n].cat, p, sizeof(t[n].cat)-1);
        t[n].cat[sizeof(t[n].cat)-1] = 0;

        p = strtok(NULL, "|"); if (!p) p = "";
        strncpy(t[n].desc, p, sizeof(t[n].desc)-1);
        t[n].desc[sizeof(t[n].desc)-1] = 0;

        p = strtok(NULL, "|");
        t[n].done = p ? atoi(p) : 0;

        n++;
    }
    fclose(fp);
}



void addTask() {
    if (n >= MAX) {
        printf("Full.\n");
        return;
    }

    inputLine("Title: ", t[n].title, sizeof(t[n].title));
    while (isBlank(t[n].title)) {
        inputLine("Title (required): ", t[n].title, sizeof(t[n].title));
    }

    inputLine("Date (YYYY-MM-DD): ", t[n].date, sizeof(t[n].date));
    inputLine("Time (HH:MM): ", t[n].time, sizeof(t[n].time));
    inputLine("Category: ", t[n].cat, sizeof(t[n].cat));
    inputLine("Description: ", t[n].desc, sizeof(t[n].desc));

    t[n].done = 0;
    n++;

    saveAll();
    printf("Added.\n");
}

void editTask() {
    if (n == 0) {
        printf("No tasks.\n");
        return;
    }

    int idx;
    printf("Edit task number: ");

    if (scanf("%d", &idx) != 1) {
        while (getchar() != '\n') {
        }
        return;
    }

    while (getchar() != '\n') {
    }

    if (idx < 1 || idx > n) {
        printf("Out of range.\n");
        return;
    }

    Task *p = &t[idx - 1];
    char buf[256];

    printf("Press ENTER to keep old value.\n");

    inputLine("New title: ", buf, sizeof(buf));
    if (!isBlank(buf)) strcpy(p->title, buf);

    inputLine("New date: ", buf, sizeof(buf));
    if (!isBlank(buf)) strcpy(p->date, buf);

    inputLine("New time: ", buf, sizeof(buf));
    if (!isBlank(buf)) strcpy(p->time, buf);

    inputLine("New category: ", buf, sizeof(buf));
    if (!isBlank(buf)) strcpy(p->cat, buf);

    inputLine("New description: ", buf, sizeof(buf));
    if (!isBlank(buf)) strcpy(p->desc, buf);

    saveAll();
    printf("Updated.\n");
}


void listTask() {
    if (n == 0) {
        printf("No tasks.\n");
        return;
    }

    printf("\n# | Title                          | Date       Time  | Category        | Done\n");
    printf("--+--------------------------------+-----------+------+----------------+------\n");

    for (int i = 0; i < n; i++) {
        printf("%2d| %-30s | %-10s %-5s | %-14s |  %c\n",
            i + 1,
            t[i].title,
            t[i].date,
            t[i].time,
            t[i].cat,
            t[i].done ? 'X' : ' '
        );
    }
}


void overdueTask() {
    if (n == 0) {
        printf("No tasks.\n");
        return;
    }

    char now[32];
    int found = 0;

    inputLine("Enter current date and time (YYYY-MM-DD HH:MM): ",
              now, sizeof(now));

    while (isBlank(now)) {
        inputLine("Enter current date and time (YYYY-MM-DD HH:MM): ",
                  now, sizeof(now));
    }

    printf("\nDeadline Tasks (Not Done):\n");
    printf("# | Title                          | Date       Time  | Category\n");
    printf("--+--------------------------------+-----------+------+----------------\n");

    for (int i = 0; i < n; i++) {
        char taskDT[32];
        strcpy(taskDT, t[i].date);
        strcat(taskDT, " ");
        strcat(taskDT, t[i].time);

        if (t[i].done == 0 && strcmp(taskDT, now) < 0) {
            printf("%2d| %-30s | %-10s %-5s | %-14s\n",
                   i + 1,
                   t[i].title,
                   t[i].date,
                   t[i].time,
                   t[i].cat);
            found = 1;
        }
    }

    if (!found) {
        printf("No deadline tasks.\n");
    }
}



void viewTask() {
    if (n == 0) {
        printf("No tasks.\n");
        return;
    }

    int idx;
    printf("View task number: ");

    if (scanf("%d", &idx) != 1) {
        while (getchar() != '\n') {
        }
        return;
    }

    while (getchar() != '\n') {
    }

    if (idx < 1 || idx > n) {
        printf("Out of range.\n");
        return;
    }

    Task *p = &t[idx - 1];

    printf("\n--- Task %d ---\n", idx);
    printf("Title      : %s\n", p->title);
    printf("Date       : %s\n", p->date);
    printf("Time       : %s\n", p->time);
    printf("Category   : %s\n", p->cat);
    printf("Done       : %s\n", p->done ? "Yes" : "No");
    printf("Description: %s\n\n", p->desc[0] ? p->desc : "(none)");
}



void delTask() {
    if (n == 0) {
        printf("No tasks.\n");
        return;
    }

    int idx;
    printf("Delete task number: ");

    if (scanf("%d", &idx) != 1) {
        while (getchar() != '\n') {
 
        }
        return;
    }

    while (getchar() != '\n') {

    }

    if (idx < 1 || idx > n) {
        printf("Out of range.\n");
        return;
    }

    for (int i = idx; i < n; i++) {
        t[i - 1] = t[i];
    }

    n--;
    saveAll();
    printf("Deleted.\n");
}

void doneTask() {
    if (n == 0) {
        printf("No tasks.\n");
        return;
    }

    int idx;
    printf("Mark done number: ");

    if (scanf("%d", &idx) != 1) {
        while (getchar() != '\n') {

        }
        return;
    }

    while (getchar() != '\n') {

    }

    if (idx < 1 || idx > n) {
        printf("Out of range.\n");
        return;
    }

    t[idx - 1].done = 1;
    saveAll();
    printf("Done.\n");
}

int main() {
    loadAll();

    while (1) {
        int ch;

        printf("\n=====================================\n");
        printf("           TASK PLANNER\n");
        printf("=====================================\n");
        printf(" 1) Add task\n");
        printf(" 2) List tasks\n");
        printf(" 3) View task details\n");
        printf(" 4) Edit task\n");
        printf(" 5) Delete task\n");
        printf(" 6) Mark task as done\n");
        printf(" 7) Deadline tasks\n");
        printf(" 8) Exit\n");
        printf("-------------------------------------\n");
        printf(" Select option: ");

        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n') {
               
            }
            printf(" Invalid input.\n");
            continue;
        }

        while (getchar() != '\n') {
            
        }

        printf("\n");

        switch (ch) {
    case 1:
        addTask();
        break;

    case 2:
        listTask();
        break;

    case 3:
        viewTask();
        break;

    case 4:
        editTask();
        break;

    case 5:
        delTask();
        break;

    case 6:
        doneTask();
        break;

    case 7:
        overdueTask();
        break;

    case 8:
        return 0;

    default:
        printf(" Invalid option.\n");
        break;
}

    }

    printf("\nThank you for using Task Planner.\n");
    return 0;
}
