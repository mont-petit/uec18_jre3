#include <stdio.h>
#include <stdlib.h>

typedef char nodeelemtype;

struct node {
    nodeelemtype element;
    struct node* next;
};
typedef struct node* list;

void print_char_list(list l);
void insert(list l, nodeelemtype e);
list tail(list l);
void parenMoveToHead(list insPos, list begin, list end);
void bracketMoveToTail(list insPos, list begin, list end);

int main() {
    char c, buf[128];
    int i;
    list l, current;
    l = (struct node*)malloc(sizeof(struct node));
    current = l;

    // constructing list
    if( fgets(buf, sizeof(buf), stdin) != NULL ) {
        for(i = 0; ( c = buf[i] ) != '\n'; ++i) {
            insert(current, c);
            current = current->next;
        }
    }

    // converting nodes from the beginning
    current = l;
    list bracketInsPos = NULL;
    list parenInsPos = l;
    int flag = 1;
    while( current->next != bracketInsPos ) {
        if( current->next->element == '(' ) {
            list begin = current, end = begin->next;
            list tmp = begin->next;
            while( tmp->element != ')' ) {
                if( tmp->next->element == '[') {
                    if( flag ) {
                        bracketInsPos = tmp->next;
                        flag = 0;
                    }
                    list b = tmp, e = b->next;
                    while( e->element != ']' ) e = e->next;
                    bracketMoveToTail(tail(end), b, e);
                }
                else tmp = tmp->next;
            }
            end = tmp;
            current = end;
            parenMoveToHead(parenInsPos, begin, end);
            parenInsPos = current;
        }
        else if( current->next->element == '[' ) {
            if( flag ) {
                bracketInsPos = current->next;
                flag = 0;
            }
            list begin = current, end = begin->next;
            list tmp = begin->next;
            while( tmp->element != ']' ) {
                if( tmp->next->element == '(' ) {
                    list b = tmp, e = b->next;
                    while( e->element != ')' ) e = e->next;
                    tmp = e;
                    parenMoveToHead(parenInsPos, b, e);
                    parenInsPos = tmp;
                }
                else tmp = tmp->next;
            }
            end = tmp;
            // for begin 
            if ( begin == l ) {
                while( begin->next->element != '[' ) begin = begin->next;
                current = begin;
            }
            bracketMoveToTail(tail(end), begin, end);
        }
        else current = current->next;
    }

    print_char_list(l);
    return 0;
}

void print_char_list(list l) {

    list tmp = l->next;
    while( tmp != NULL ) {
        switch( tmp->element ) {
            case '[': break; //Do Nothing
            case ']': break; //Do Nothing
            case '(': break; //Do Nothing
            case ')': break; //Do Nothing
            default:
                printf("%c", tmp->element);
                break;
        }

        tmp = tmp->next;
    }

    printf("\n");
}

void insert(list l, nodeelemtype e) {
    struct node* n;
    n = (struct node*)malloc(sizeof(struct node));

    n->next = l->next;
    n->element = e;
    l->next = n;
}

list tail(list l) {
    list res = l;

    while( res->next != NULL ) res = res->next;
    return res;
}

/**
 * リストの整列済み位置, '('の位置と')'の位置を受け取り, リストを繋ぎ変える.
 */
void parenMoveToHead(list insPos, list begin, list end) {
    if( insPos != begin) {
        list tmp = begin->next;
        begin->next = end->next;
        end->next = insPos->next;
        insPos->next = tmp;
    }
}

/**
 * リストの挿入位置、'['の位置と']'の位置を受け取り、リストを繋ぎ変える.
 */
void bracketMoveToTail(list insPos, list begin, list end) {
    if( insPos != end ) {
        list tmp = end->next;
        end->next = insPos->next;
        insPos->next = begin->next;
        begin->next = tmp;
    }
}
