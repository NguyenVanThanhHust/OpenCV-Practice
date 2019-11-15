#include "train_HOG.h"

int main() {
    bool bTrainedHog = true;
    if (bTrainedHog) {
        CTrainedHog trainedHog;
        trainedHog.run_trained_hog();
    }

    getchar();

    return 0;
}
