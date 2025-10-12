#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// store user data in "users.txt".
#define FILENAME "users.txt"

// user
struct User
{
  int id;
  char name[50];
  int age;
};

// add user
void addUser()
{
  struct User newUser;

  printf("Enter User ID: ");
  scanf("%d", &newUser.id);

  printf("Enter User Name: ");
  scanf("%s", newUser.name);

  printf("Enter User Age: ");
  scanf("%d", &newUser.age);

  FILE *file = fopen(FILENAME, "a");

  if (file == NULL)
  {
    printf("Error: Could not open the file.\n");
    return;
  }

  fprintf(file, "%d %s %d\n", newUser.id, newUser.name, newUser.age);

  fclose(file);

  printf("User added successfully!\n");
}

// display all users
void displayUsers()
{

  FILE *file = fopen(FILENAME, "r");

  if (file == NULL)
  {
    printf("No users found. The file might be empty or missing.\n");
    return;
  }

  struct User currentUser;

  printf("\n--- List of All Users ---\n");
  printf("ID\tName\tAge\n");
  printf("-------------------------\n");

  while (fscanf(file, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) != EOF)
  {
    printf("%d\t%s\t%d\n", currentUser.id, currentUser.name, currentUser.age);
  }
  printf("-------------------------\n");

  fclose(file);
}

// update user
void updateUser()
{
  int idToUpdate;
  printf("Enter the ID of the user you want to update: ");
  scanf("%d", &idToUpdate);

  // create new file, update that file and replace with original
  FILE *originalFile = fopen(FILENAME, "r");
  FILE *tempFile = fopen("temp.txt", "w");

  // Check if both files opened correctly.
  if (originalFile == NULL || tempFile == NULL)
  {
    printf("Error opening files.\n");
    return;
  }

  struct User user;
  int found = 0;

  while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) != EOF)
  {
    if (found == 0 && user.id == idToUpdate)
    {
      found = 1;
      printf("Enter the new name: ");
      scanf("%s", user.name);
      printf("Enter the new age: ");
      scanf("%d", &user.age);
    }

    fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
  }

  fclose(originalFile);
  fclose(tempFile);

  // replace the old file with new
  remove(FILENAME);
  rename("temp.txt", FILENAME);

  if (found)
  {
    printf("User updated successfully.\n");
  }
  else
  {
    printf("User with ID %d was not found.\n", idToUpdate);
  }
}

// delete user
void deleteUser()
{
  int idToDelete;
  printf("Enter the ID of the user you want to delete: ");
  scanf("%d", &idToDelete);

  // similar to updating.
  FILE *originalFile = fopen(FILENAME, "r");
  FILE *tempFile = fopen("temp.txt", "w");

  if (originalFile == NULL || tempFile == NULL)
  {
    printf("Error opening files.\n");
    return;
  }

  struct User user;
  int found = 0;

  while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) != EOF)
  {
    // add if the user id is not according to delete id
    if (user.id != idToDelete)
    {
      fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
    }
    else
    {
      // else do not add
      found = 1;
    }
  }

  fclose(originalFile);
  fclose(tempFile);

  // Replace the old file with the new
  remove(FILENAME);
  rename("temp.txt", FILENAME);

  if (found)
  {
    printf("User deleted successfully.\n");
  }
  else
  {
    printf("User with ID %d was not found.\n", idToDelete);
  }
}

// main method
int main()
{
  int choice = 0;
  // ask for choice
  do
  {
    printf("\n--- User Management Menu ---\n");
    printf("1. Add a New User\n");
    printf("2. Show All Users\n");
    printf("3. Update a User\n");
    printf("4. Delete a User\n");
    printf("5. Exit\n");
    printf("----------------------------\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      addUser();
      break;
    case 2:
      displayUsers();
      break;
    case 3:
      updateUser();
      break;
    case 4:
      deleteUser();
      break;
    case 5:
      break;
    default:
      printf("That's not a valid option. Please try again.\n");
    }
  } while (choice != 5);

  return 0;
}
