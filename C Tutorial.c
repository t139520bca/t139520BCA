#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SESSIONS 100

typedef struct {
    int id;
    char subject[50];
    int minutes;
    char note[150];
    char date[20];
} StudySession;

StudySession sessions[MAX_SESSIONS];
int sessionCount = 0;

void clearInput()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void getDate(char date[])
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(date, 20, "%d-%m-%Y", t);
}

void saveData()
{
    FILE *file = fopen("sessions.dat", "wb");

    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    fwrite(&sessionCount, sizeof(int), 1, file);
    fwrite(sessions, sizeof(StudySession), sessionCount, file);

    fclose(file);
}

void loadData()
{
    FILE *file = fopen("sessions.dat", "rb");

    if (file == NULL)
        return;

    fread(&sessionCount, sizeof(int), 1, file);
    fread(sessions, sizeof(StudySession), sessionCount, file);

    fclose(file);
}

void addSession()
{
    if (sessionCount >= MAX_SESSIONS) {
        printf("\nSession limit reached!\n");
        return;
    }

    sessions[sessionCount].id = sessionCount + 1;

    printf("\nEnter subject: ");
    fgets(sessions[sessionCount].subject, 50, stdin);

    sessions[sessionCount].subject[
        strcspn(sessions[sessionCount].subject, "\n")
    ] = '\0';

    printf("Enter study time in minutes: ");

    if (scanf("%d", &sessions[sessionCount].minutes) != 1) {
        printf("Invalid input!\n");
        clearInput();
        return;
    }

    clearInput();

    if (sessions[sessionCount].minutes <= 0) {
        printf("Time must be greater than 0.\n");
        return;
    }

    printf("Enter note: ");
    fgets(sessions[sessionCount].note, 150, stdin);

    sessions[sessionCount].note[
        strcspn(sessions[sessionCount].note, "\n")
    ] = '\0';

    getDate(sessions[sessionCount].date);

    sessionCount++;

    saveData();

    printf("\nSession added successfully!\n");
}

void viewSessions()
{
    int i;

    if (sessionCount == 0) {
        printf("\nNo sessions available.\n");
        return;
    }

    printf("\n========== STUDY SESSIONS ==========\n");

    for (i = 0; i < sessionCount; i++) {
        printf("\nID      : %d\n", sessions[i].id);
        printf("Subject : %s\n", sessions[i].subject);
        printf("Time    : %d minutes\n", sessions[i].minutes);
        printf("Date    : %s\n", sessions[i].date);
        printf("Note    : %s\n", sessions[i].note);
        printf("------------------------------------\n");
    }
}

void searchSubject()
{
    char search[50];
    int found = 0;
    int i;

    printf("\nEnter subject to search: ");
    fgets(search, 50, stdin);

    search[strcspn(search, "\n")] = '\0';

    for (i = 0; i < sessionCount; i++) {

        if (strstr(sessions[i].subject, search) != NULL) {

            printf("\nID      : %d\n", sessions[i].id);
            printf("Subject : %s\n", sessions[i].subject);
            printf("Time    : %d minutes\n", sessions[i].minutes);
            printf("Date    : %s\n", sessions[i].date);

            found = 1;
        }
    }

    if (!found)
        printf("\nNo matching subject found.\n");
}

void statistics()
{
    int i;
    int total = 0;

    if (sessionCount == 0) {
        printf("\nNo study data available.\n");
        return;
    }

    for (i = 0; i < sessionCount; i++)
        total += sessions[i].minutes;

    printf("\n========== STATISTICS ==========\n");
    printf("Total sessions : %d\n", sessionCount);
    printf("Total minutes  : %d\n", total);
    printf("Total hours    : %.2f\n", total / 60.0);
    printf("Average session: %.2f minutes\n",
           (float)total / sessionCount);
    printf("================================\n");
}

void deleteSession()
{
    int id;
    int index = -1;
    int i;

    if (sessionCount == 0) {
        printf("\nNo sessions available.\n");
        return;
    }

    printf("\nEnter session ID to delete: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        clearInput();
        return;
    }

    clearInput();

    for (i = 0; i < sessionCount; i++) {
        if (sessions[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nSession not found.\n");
        return;
    }

    for (i = index; i < sessionCount - 1; i++) {
        sessions[i] = sessions[i + 1];
        sessions[i].id = i + 1;
    }

    sessionCount--;

    saveData();

    printf("\nSession deleted successfully!\n");
}

void menu()
{
    printf("\n");
    printf("====================================\n");
    printf("          FOCUSFLOW\n");
    printf("       Study Tracker in C\n");
    printf("====================================\n");
    printf("1. Add Study Session\n");
    printf("2. View Sessions\n");
    printf("3. Search Subject\n");
    printf("4. Statistics\n");
    printf("5. Delete Session\n");
    printf("6. Exit\n");
    printf("====================================\n");
}

int main()
{
    int choice;

    loadData();

    printf("\nWelcome to FocusFlow!\n");

    while (1) {

        menu();

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nPlease enter a number.\n");
            clearInput();
            continue;
        }

        clearInput();

        switch (choice) {

            case 1:
                addSession();
                break;

            case 2:
                viewSessions();
                break;

            case 3:
                searchSubject();
                break;

            case 4:
                statistics();
                break;

            case 5:
                deleteSession();
                break;

            case 6:
                printf("\nThank you for using FocusFlow!\n");
                return 0;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}
