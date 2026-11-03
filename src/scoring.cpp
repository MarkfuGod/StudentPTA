#include "scoring.h"

#include <algorithm>

namespace {

// Index of the first limit >= v, i.e. the tier v belongs to.
template <size_t N>
int tierOf(int v, const int (&limits)[N]) {
    return static_cast<int>(std::lower_bound(limits, limits + N, v) - limits);
}

}  // namespace

ScoreConfig defaultScoreConfig() {
    ScoreConfig c = {
        {25, 20, 10, 0},  // time: <=500s, <=1000s, <=3000s, longer
        {25, 20, 10},     // wrong: 0, 1-3, more than 3
        {25, 20, 10},     // duplicate: <=20%, <=35%, <=50%
    };
    return c;
}

int timeTier(int seconds) { return tierOf(seconds, kTimeLimits); }
int wrongTier(int wrongTimes) { return tierOf(wrongTimes, kWrongLimits); }
int duplicateTier(int duplicateRate) { return tierOf(duplicateRate, kDuplicateLimits); }

bool isValidProblem(char problem) {
    return problem >= 'A' && problem < 'A' + kProblemCount;
}

double scoreRecord(const Record& r, const ScoreConfig& config) {
    if (!r.passed || r.duplicateRate > kMaxDuplicateRate || !isValidProblem(r.problem))
        return 0.0;
    int raw = config.time[timeTier(r.time)]
            + config.wrong[wrongTier(r.wrongTimes)]
            + config.duplicate[duplicateTier(r.duplicateRate)]
            + kPassScore;
    return raw * kProblemWeight[r.problem - 'A'];
}
