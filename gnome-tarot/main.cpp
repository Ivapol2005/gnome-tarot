#include <cstdio>
#include <cstring>
#include <random>

//s
class card {
public:
    int id=0;
    int ifReversed=0;
    char name[128]="";
    char description[1024]="";

    card(int id1, int ifReversed1, const char *name1, const char *description1) {
        ifReversed = ifReversed1;
        id = id1;
        strncpy(name, name1, sizeof(name)-1);
        strncpy(description, description1, sizeof(description)-1);
    }

    void reset() {
        id = 0;
        memset(name, 0, sizeof(name));
        memset(description, 0, sizeof(description));
    }

};


struct deck{
    card card1;
    deck *next = nullptr;

    deck() : card1(0, 0, "", "") {}
};
deck *head = nullptr;


struct deckList{
    card card1;
    deckList *deckListNext = nullptr;

    deckList() : card1(0, 0, "", "") {}
};
deckList *deckListHead = nullptr;


//void to sort cards
void sortCards(int numberOfCards, int deckSort[]){
    std::random_device rd;
    std::mt19937 gen(rd());
    //sort cards to random
    //int deckSort[numberOfCards];

    for(int i=0; i<numberOfCards; i++){
        deckSort[i]=i;
    }

    for(int i=numberOfCards - 1; i > 0; i--){
        std::uniform_int_distribution<> dist(0, i+1);
        int j = dist(gen);
        if (i != j){
            int temp = deckSort[i];
            deckSort[i] = deckSort[j];
            deckSort[j] = temp;
        }
    }
}


//add card to the end of  deck
void addCardToEnd(card card) {
    auto *newNode = new deck;
    newNode->card1 = card;
    newNode->next = nullptr;

    if (!head) {
        head = newNode;
    } else {
        deck *tail = head;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = newNode;
    }
}


void createDeck(){

    //find number of cards

    //char buffer[1024];//buffer saves line
    int numberOfCards = 77;


    //add card names from file
    const char* cardName_file_name = "cards/names.txt";
    FILE* file = fopen(cardName_file_name, "r");

    if (file == nullptr) {
        fprintf(stderr, "can't open file: %s\n", cardName_file_name);
    }

    char buffer[64];
    char* cardNames[numberOfCards];
    int num_lines=0;


    while (fgets(buffer, sizeof(buffer), file) != nullptr) {
        //for (int i=0; i<129; i++) {

        //change \n to \0
        int len = strlen(buffer);
        int i = 0;
        while (i < len && buffer[i] != '\n') {
            i++;
        }

        if (buffer[i] == '\n') {
            buffer[i] = '\0';
        }

        //add to list of names
        cardNames[num_lines] = strdup(buffer);
        num_lines++;
    }
    fclose(file);


    //add card descriptions from file
    const char* cardDescription_file_name = "cards/description_straight.txt";
    FILE* file1 = fopen(cardDescription_file_name, "r");

    if (file1 == nullptr) {
        fprintf(stderr, "can't open file: %s\n", cardDescription_file_name);
    }
    char buffer1[512];  // Збільшив розмір буфера
    char* cardDescriptions[numberOfCards];
    int num_lines1 = 0;

    while (fgets(buffer1, sizeof(buffer1), file1) != nullptr) {
        int len = strlen(buffer1);
        int i = 0;
        while (i < len && buffer1[i] != '\n') {
            i++;
        }

        if (buffer1[i] == '\n') {
            buffer1[i] = '\0';
        }

        // add to list of descriptions
        cardDescriptions[num_lines1] = strdup(buffer1);
        num_lines1++;
    }
    fclose(file1);


    //add card descriptions from file
    const char* cardDescriptionRev_file_name = "cards/description_reversed.txt";
    FILE* file2 = fopen(cardDescriptionRev_file_name, "r");

    if (file2 == nullptr) {
        fprintf(stderr, "can't open file: %s\n", cardDescriptionRev_file_name);
    }
    char buffer2[512];  // Збільшив розмір буфера
    char* cardDescriptionsRev[numberOfCards];
    int num_lines2 = 0;

    while (fgets(buffer2, sizeof(buffer2), file2) != nullptr) {

        // change \n to \0
        int len = strlen(buffer2);
        int i = 0;
        while (i < len && buffer2[i] != '\n') {
            i++;
        }

        if (buffer2[i] == '\n') {
            buffer2[i] = '\0';
        }

        // add to list of descriptions
        cardDescriptionsRev[num_lines2] = strdup(buffer2);
        num_lines2++;
    }
    fclose(file2);



    //scrypt creates objects and fins out number of cards
    for (int i=0; i<num_lines; i++) {
        //create object
        //card card(i, "cardName", "cardDescription");

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dist(0, 1);
        int j = dist(gen);


        char description_choose[512];
        if(j==0){
            //description_choose = cardDescriptions[i];
            strncpy(description_choose, cardDescriptions[i], sizeof(description_choose)-1);
        }else{
            //description_choose = cardDescriptionsRev[i];
            strncpy(description_choose, cardDescriptionsRev[i], sizeof(description_choose)-1);
        }

        card card1(i, j, cardNames[i], description_choose);

        deckList *n;
        if (!deckListHead)
        {
            deckListHead = new deckList;
            deckListHead->card1 = card1;
            deckListHead->deckListNext = nullptr;
            //return;
        }
        else
        {
            n = deckListHead;
            while (n->deckListNext)
            {
                n = n->deckListNext;
            }
            auto * newNode = new deckList;
            newNode->card1 = card1;
            newNode->deckListNext = nullptr;
            n->deckListNext = newNode;
            //return;
        }

        numberOfCards++;

    }


    int deckSort[numberOfCards];
    sortCards(numberOfCards, deckSort);

    //add cards to linked list deck
    for(int i=0; i<numberOfCards; i++){
        //find card in deckList
        int searchId = deckSort[i];
        deckList* current = deckListHead;

        while (current != nullptr) {
            if (current->card1.id == searchId) {
                // found element
                card newCard(searchId, current->card1.ifReversed, current->card1.name, current->card1.description);
                addCardToEnd(newCard);
            }
            current = current->deckListNext;
        }
    }


}


//new game
void newGame(){
    printf("ok");
    //at first i want to sort s

    createDeck();
}


void menuChooseVoid(int menuChoose){

    switch(menuChoose){
        case 1: {
            //show one card
            int choose_card_number;
            printf("choose card number. 1 to 78\n");
            scanf("%i", &choose_card_number);

            if(choose_card_number<1 || choose_card_number>130){

            }

            choose_card_number-=1;

            deck *deckCurrent = head;
            for(int i=0; i< choose_card_number; i++){
                deckCurrent = deckCurrent->next;
            }

            if(deckCurrent->card1.name == ""){
                printf("you already chosed this card");

                menuChoose = 0;
                printf("  what to do?\n1-take card\n2-see your cards\n3-new deck\n");
                scanf("%i", &menuChoose);

                menuChooseVoid(menuChoose);
            }

            //printf("Card ID: %d\n", deckCurrent->card1.id);
            printf("Card Name: %s\n", deckCurrent->card1.name);
            if(deckCurrent->card1.ifReversed == 1){
                printf("reversed\n");
            }else{
                printf("straight\n");
            }
            printf("Card Description: %s\n", deckCurrent->card1.description);
            printf("\n");

            deckCurrent->card1.reset();


            menuChoose = 0;
            printf("  what to do?\n1-take card\n2-see your cards\n3-new deck\n");
            scanf("%i", &menuChoose);

            menuChooseVoid(menuChoose);

            break;
        }


        case 2: {
            //print sorted list
            deck *deckCurrent = head;

            while (deckCurrent != nullptr) {
                printf("Card ID: %d\n", deckCurrent->card1.id);
                printf("Card Name: %s\n", deckCurrent->card1.name);
                if(deckCurrent->card1.ifReversed == 1){
                    printf("reversed\n");
                }else{
                    printf("straight\n");
                }
                printf("Card Description: %s\n", deckCurrent->card1.description);
                printf("\n");

                deckCurrent = deckCurrent->next;
            }

            menuChoose = 0;
            printf("  what to do?\n1-take card\n2-see your cards\n3-new deck\n");
            scanf("%i", &menuChoose);

            menuChooseVoid(menuChoose);

            break;
        }


        case 3: {
            newGame();
            printf("complete");

            menuChoose = 0;
            printf("  what to do?\n1-take card\n2-see your cards\n3-new deck\n");
            scanf("%i", &menuChoose);

            menuChooseVoid(menuChoose);
        }


        default:
            //other
            printf("don't know command");

            menuChoose = 0;
            printf("  what to do?\n1-take card\n2-see your cards\n3-new deck\n");
            scanf("%i", &menuChoose);

            menuChooseVoid(menuChoose);

            break;
    }

}


int main() {
    int menuChoose;
    newGame();

    //menu
    printf("  what to do?\n1-take card\n2-see your cards\n3-new deck\n4-exit\n");
    scanf("%i", &menuChoose);


    printf("\n");

    menuChooseVoid(menuChoose);


    //delete deck
    deck* deckCurrent = head;
    while (deckCurrent != nullptr) {
        deck* temp = deckCurrent;
        deckCurrent = deckCurrent->next;
        delete temp;
    }
    head = nullptr;


    //delete deckList
    deckList* deckListCurrent = deckListHead;
    while (deckListCurrent != nullptr) {
        deckList* temp = deckListCurrent;
        deckListCurrent = deckListCurrent->deckListNext;
        delete temp;
    }
    deckListHead = nullptr;

    return 0;
}