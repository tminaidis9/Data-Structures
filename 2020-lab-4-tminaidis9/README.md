![run-tests](../../workflows/run-tests/badge.svg)

## Δομές Δεδομένων και Τεχνικές Προγραμματισμού

### Κώδικας Εργαστηρίου 4

Εκφώνηση: https://k08.chatzi.org/labs/lab4/ 

# Η συνάρτηση naive_heapify έχει πολυπλοκότητα Ο(nlogn) καθώς η for που υπάρχει για να εντάσσει τα στοιχεία στην στοίβα γίνεται n φορές και after bubble up με πολυπλοκοτητα Ο(logn) και τελικά έχει best case O(n) worst και average case O(nlogn).
############################################################### 
# η νέα naive_heapify έχει πολυπλοκότητα Ο(n) average case.
############################################################### 
# Στην ADTPQ μέσω λίστας η create έχει πολυπλοκότητα Ο(nlogn), η pqueue_size, pqueue_remove_max,set_destroy_value και pqueue_max O(1),η insert έχει Ο(n) worst και average case.Tέλος η pqueue_destroy έχει πολυπλ. Ο(n).

# Στην ADTPQ μέσω σωρού η create έχει πολυπλοκότητα Ο(n) άν το vector != NULL, η pqueue_size, pqueue_remove_max,set_destroy_value και pqueue_max O(1),η insert έχει πολυπλοκότητα Ο(logn).Tέλος η pqueue_destroy έχει πολυπλ. Ο(n).

# Συγκριτικά, η υλοποίηση μέσω σωρού υπερτερεί σε πολυπλοκότητα στην δημιουργία του ADT και στην insert.