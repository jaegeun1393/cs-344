#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {//set up the blank struct to contain the data
  char *title;
  int year;
  char *languages;
  float rating;

  struct movie *next;
};

struct movie *createList(char *currLine) {
  struct movie *currmovie = malloc(sizeof(struct movie));
  char *saveptr;

  char *temp = strtok_r(currLine, ",", &saveptr); //title of the movie save data into saveptr 
  currmovie->title = calloc(strlen(temp) + 1, sizeof(char));
  strcpy(currmovie->title, temp);

  temp = strtok_r(NULL, ",", &saveptr); // year
  currmovie->year = atoi(temp); //character to int

  temp = strtok_r(NULL, ",", &saveptr); //lang
  currmovie->languages = calloc(strlen(temp) + 1, sizeof(char));
  strcpy(currmovie->languages, temp);

  temp = strtok_r(NULL, ",\n", &saveptr); // rating
  currmovie->rating = atof(temp);

  currmovie->next = NULL;
  return currmovie;
}

struct movie *processFile (char *file, int *cnt) { //link file
  FILE *data = fopen(file,"r"); //data

  char line[100];
  int cnt2 = 0;

  struct movie *head = NULL; //create head for the link
  struct movie *tail = NULL; //tail

  if(data != NULL) { //check the data is real
    fgets(line, sizeof line, data); //skip the first line
    while(fgets(line, sizeof line, data) != NULL) { //loop until the file hits the blank
      struct movie *node = createList(line);

      if(head == NULL) { 
        head = node;
        tail = node;
      } else {
        tail->next = node;
        tail = node;
      }
      cnt2++; // to count the number of movie the file has
    }
  }

  fclose(data); //cloase the file
  printf("Processed file movie_sample_1.csv and parsed data for %d movies\n", cnt2);
  *cnt = cnt2;
  return head;
}

void print_question() {
  printf("1. Show movies released in the specified year.\n");
  printf("2. Show highest rated movie for each year.\n");
  printf("3. Show the title and year of release of all movies in a specific language.\n");
  printf("4. Exit from the program.\n\n");
  printf("Enter a number between 1-4: ");
}

int enter() {//function to recieve the user input
  int input;
  scanf("%d", &input);
  return input;
}

void movie_year(struct movie *mv) {
  int input, i = 0;

  printf("Enter the year for which you want to see movies: ");
  input = enter(); //get the input from the above function

  while(mv != NULL) {
    if(mv->year == input) { //check out movies that has same year that the user entered
      printf("%s\n", mv->title); //if so, print out
      i++; //make the mark that the data has the info
    }
    mv = mv->next;
  }
  if(i == 0) { //if not then print out the different result
    printf("No data about movies released in the year %d\n\n", input);
  } else {
  printf("\n"); }
}

void comp(struct movie *mv) {
  struct movie *temp = mv;

  int check = 0;
  if(mv->next != NULL) { //check the struct is the last one
    mv = mv->next;
    while(mv != NULL) { //compare until the struct is over
      if(temp->year == mv->year) { //if year is same
        check = 1;
        if(temp->rating > mv->rating) { //once the program find the year printing out highest rate result
          printf("%d %.2f %s\n", temp->year, temp->rating ,temp->title);
          break;
        } else {
          printf("%d %.2f %s\n", mv->year, mv->rating ,mv->title);
          break;
        }
      }
      mv = mv->next;
    }
  }
  if(check == 0) { //case for only one movie in a specfic year
        printf("%d %.2f %s\n", temp->year, temp->rating ,temp->title);
  }
}

void rated(struct movie *mv, int num) { //for find the highest rate
  int year[100], i = 0, k = 0; //year for save years, i for countuin, and k for checking any duplication
  comp(mv);
  year[i] = mv->year; //save the first year into the array
  i++;

  while(mv != NULL) {
    for(int j = 0; j < i; j++) {
      if(mv->year == year[j]) { //check if there is any same year
        k = 1;
      }
    }

    if(k == 1) { //if the computer find the same year then move to the next
      mv = mv->next;
    } else { //if not then save year to the value
      comp(mv); // run to find the high rate
      year[i] = mv->year;
      i++;
      mv = mv->next;
    }
    k = 0; //reset the status
  }
  printf("\n");
}

void check_lang(struct movie *mv, char lang[], int *not) { //check out the same language
  char mv_lang[strlen(mv->languages)], tmp[strlen(mv->languages)];
  int j = 0;

  strcpy(mv_lang, mv->languages); //cpy the original
  memmove(mv_lang, mv_lang+1, strlen(mv_lang)); //Remove first [
  mv_lang[strlen(mv_lang)-1] = '\0'; //remove last ]

      for (int k = 0; k < strlen(mv->languages); k++) {tmp[k] = '\0';} //after remove (), [], or ; check out the word by letter. Like "E", "n"...."h" if same as user input then do the work
      j = 0;

  for(int i = 0; i < strlen(mv_lang); i++) {
    if(mv_lang[i] != ';') {
      tmp[j] = mv_lang[i];
      j++;

      if (strcmp(lang, tmp) == 0) {printf("%d %s\n", mv->year, mv->title); *not = 1;} //if find the result then make the value n to 1
    } else {
      for (int k = 0; k < strlen(mv->languages); k++) {tmp[k] = '\0';} //if not then reset the save word for the next word
      j = 0;
    }
  }
}

void find_lang(struct movie *mv, int cnt, char lang[]) {
  int not = 0;
    while(mv != NULL) { //until the movie list is over
      check_lang(mv, lang, &not);//bring the input and the user input
      mv = mv->next;
    }
    if(not == 0) {//if n is 0 from the check_lang function then return the fault message.
    printf("No data about movies released in %s\n", lang);
  }
    printf("\n");
}

void start_program(struct movie *mv, int cnt) {

  int input = 0;
  while(input != 4) { //looping until the user hits the number 4
    print_question(); //print out the options
    input = enter(); //get the user input
    
    if(input == 1) {
      movie_year(mv); //start find the movie that user enetered
    }
    else if(input == 2) {
      rated(mv, cnt); //run the rated funtion
    }
    else if(input == 3) {
      char lang[1000];
      printf("Enter the language for which you want to see movies: ");
      scanf("%s", lang);
      find_lang(mv, cnt, lang); //after enter the language, run the find lanugage function
    }
    else if(input == 4) {
      break; //end the program
    } else {
      printf("You entered an incorrect choice. Try again.\n\n");
    }
  }
}


int main(int argc, char *argv[]) {
  int cnt = 0;
  if (argc < 2) { //check out there is other file 
		printf("Please attach the CSV file as an input.\n");
		exit(0); //Check out the csv file
	}

  struct movie *mv = processFile(argv[1], &cnt); //make the movie struct (empty)
  start_program(mv, cnt); //and fill contents inside the empty struct
  return 0;
}