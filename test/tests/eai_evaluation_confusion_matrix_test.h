//
// Created by DavideLoconte on 25-Nov-22.
//

#ifndef EAI_TOOLBOX_EAI_EVALUATION_CONFUSION_MATRIX_H
#define EAI_TOOLBOX_EAI_EVALUATION_CONFUSION_MATRIX_H

#include <ulib.h>

void test_eai_confusion_matrix_scores_accuracy_1(void);
void test_eai_confusion_matrix_scores_accuracy_05(void);
void test_eai_confusion_matrix_scores_errors(void);

#define EAI_CONFUSION_MATRIX_TESTS                                                                 \
    test_eai_confusion_matrix_scores_accuracy_1, test_eai_confusion_matrix_scores_accuracy_05,     \
        test_eai_confusion_matrix_scores_errors

#endif // EAI_TOOLBOX_EAI_EVALUATION_CONFUSION_MATRIX_H
