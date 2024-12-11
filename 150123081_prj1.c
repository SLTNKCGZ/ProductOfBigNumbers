#include <stdio.h>
#include <stdlib.h>

typedef struct number {
    char ch;
    struct number *ptr;
} number;

typedef struct list {
    number *result;
    struct list *ptr;
} list;

number *reverseList(number *head) {
    number *prev = NULL;
    number *current = head;
    number *next = NULL;

    while (current != NULL) {
        next = current->ptr;
        current->ptr = prev;
        prev = current;
        current = next;
    }
    return prev;
}

void printList(number *head) {
    number *temp = head;
    while (temp != NULL) {
        printf("%c", temp->ch);
        temp = temp->ptr;
    }
    printf("\n");
}

number *addTwoNumbers(number *num1, number *num2) {
    number *result = NULL, *current = NULL;
    int carry = 0;

    // iki sayı aynı anda toplanacak
    while (num1 != NULL || num2 != NULL || carry > 0) {
        int sum = carry;

        // ilk sayının basamağını ekle
        if (num1 != NULL) {
            sum += num1->ch - '0';
            num1 = num1->ptr;
        }

        // ikinci sayının basamağını ekle
        if (num2 != NULL) {
            sum += num2->ch - '0';
            num2 = num2->ptr;
        }

        carry = sum / 10;  // Yeni ta��ma de�erini hesapla
        sum = sum % 10;    // Mevcut basama�� bul

        // Yeni d���m olu�tur ve sonuca ekle
        number *newNode = malloc(sizeof(number));
        newNode->ch = (char)(sum + '0');
        newNode->ptr = NULL;

        if (result == NULL) {
            result = newNode;  // �lk d���m
            current = newNode;
        } else {
            current->ptr = newNode;  // Sonraki d���m
            current = newNode;
        }
    }

    return result;  // Toplam sonucu d�nd�r
}

number *shiftAndAdd(number *num, int shift, number *result) {
    number *currentResult = result;
    number *prevResult = NULL;
    int i;
    // Ara sonucu sonuca ekle
    for (i = 0; i < shift; i++) {
        if (currentResult == NULL) {
            // E�er result listesi bitmi�se bo� d���mler ekle
            number *newNode = malloc(sizeof(number));
            newNode->ch = '0';
            newNode->ptr = NULL;
            if (prevResult != NULL) {
                prevResult->ptr = newNode;
            } else {
                result = newNode;  // �lk d���m olabilir
            }
            prevResult = newNode;
        } else {
            prevResult = currentResult;
            currentResult = currentResult->ptr;
        }
    }

    // Ara sonucu toplama ba�lat
    int carry = 0;
    number *shiftedNum = num; // Kayd�r�lm�� say�

    while (shiftedNum != NULL || carry > 0) {
        int sum = carry;

        if (shiftedNum != NULL) {
            sum += shiftedNum->ch - '0';
            shiftedNum = shiftedNum->ptr;
        }

        if (currentResult != NULL) {
            sum += currentResult->ch - '0';
            currentResult->ch = (char)(sum % 10 + '0');
            carry = sum / 10;
            currentResult = currentResult->ptr;
        } else {
            // E�er result listesi bittiyse yeni d���mler ekle
            number *newNode = malloc(sizeof(number));
            newNode->ch = (char)(sum % 10 + '0');
            newNode->ptr = NULL;
            prevResult->ptr = newNode;
            prevResult = newNode;
            carry = sum / 10;
        }
    }

    return result;
}

void multiple(number *multiplicand, number *multiplier) {
    number *finalResult = NULL;  // Toplam sonucun tutulaca�� liste
    int shift = 0;  // Basamak kayd�rmay� takip etmek i�in

    // multiplier'in her basama�� i�in d�� d�ng�
    while (multiplier != NULL) {
        if (multiplier->ch < '0' || multiplier->ch > '9') {
            multiplier = multiplier->ptr;
            continue;
        }

        // Her �arpma ad�m� i�in yeni bir result listesi olu�tur
        number *result = NULL;
        number *currentResult = NULL;
        int carry = 0;

        // multiplicand'in her basama�� ile �arpma i�lemi
        number *temp = multiplicand;
        while (temp != NULL) {
            if (temp->ch < '0' || temp->ch > '9') {
                temp = temp->ptr;
                continue;
            }

            // �arpma i�lemi ve elde hesaplama
            int r = (temp->ch - '0') * (multiplier->ch - '0') + carry;
            carry = r / 10;
            r %= 10;

            // Yeni d���m olu�tur ve result listesine ekle
            number *newNode = malloc(sizeof(number));
            newNode->ch = (char)(r + '0');
            newNode->ptr = NULL;

            if (result == NULL) {
                result = newNode;
                currentResult = newNode;
            } else {
                currentResult->ptr = newNode;
                currentResult = newNode;
            }

            temp = temp->ptr; // Bir sonraki basama�a ge�
        }

        // Kalan eldeyi ekle
        if (carry > 0) {
            number *carryNode = malloc(sizeof(number));
            carryNode->ch = (char)(carry + '0');
            carryNode->ptr = NULL;
            currentResult->ptr = carryNode;
        }

        // Ara sonucu finalResult'a ekle
        finalResult = shiftAndAdd(result, shift, finalResult);

        // multiplier'in bir sonraki basama��na ge�
        multiplier = multiplier->ptr;
        shift++;  // Sonraki ara sonu� bir basamak kayd�r�lmal�
    }

    // Sonu�lar� yazd�r
    printf("�arp�m Sonucu: ");
    printList(finalResult);
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Can not find the file.\n");
        return 0;
    }

    number *multiplicand = malloc(sizeof(number));
    number *current1 = multiplicand;
    number *multiplier = malloc(sizeof(number));
    number *current2 = multiplier;

    // �lk listeyi okuma (multiplicand)
    while ((current1->ch = fgetc(inputFile)) != '\n' && current1->ch != EOF) {
        current1->ptr = malloc(sizeof(number));
        current1 = current1->ptr;
    }
    current1->ptr = NULL;  // Listenin sonuna NULL ekle

    // �kinci listeyi okuma (multiplier)
    while ((current2->ch = fgetc(inputFile)) != '\n' && current2->ch != EOF) {
        current2->ptr = malloc(sizeof(number));
        current2 = current2->ptr;
    }
    current2->ptr = NULL;  // Listenin sonuna NULL ekle

    multiplicand = reverseList(multiplicand);
    multiplier = reverseList(multiplier);

    printf("Reversed multiplicand: ");
    printList(multiplicand);  // Ters �evrilmi� ilk listeyi yazd�r

    printf("Reversed multiplier: ");
    printList(multiplier);    // Ters �evrilmi� ikinci listeyi yazd�r
    
    multiple(multiplicand, multiplier);
    
    fclose(inputFile);
    return 0;
}

