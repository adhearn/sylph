#include <stdlib.h>
#include "list.h"
#include "unity.h"

void test_EmptyList() {
    List *ls = List_new(free);
    TEST_ASSERT_NOT_NULL(ls);
    TEST_ASSERT_NULL(List_head(ls));
    List_free(ls);
}

void test_AppendList() {
    List *ls = List_new(NULL);
    List_append(ls, 3);
    List_append(ls, 7);
    TEST_ASSERT_EQUAL(3, List_head(ls));
    List_free(ls);
}

void test_PopList() {
    List *ls = List_new(NULL);
    List_append(ls, 3);
    List_append(ls, 7);
    List_append(ls, 2);
    int head = List_pop(ls);
    TEST_ASSERT_EQUAL(3, head);
    TEST_ASSERT_EQUAL(7, List_head(ls));
    head = List_pop(ls);
    TEST_ASSERT_EQUAL(7, head);
    TEST_ASSERT_EQUAL(2, List_head(ls));
    head = List_pop(ls);
    TEST_ASSERT_EQUAL(2, head);
    TEST_ASSERT_NULL(List_head(ls));
    List_free(ls);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_EmptyList);
    RUN_TEST(test_AppendList);
    RUN_TEST(test_PopList);
    return UNITY_END();
}
