#ifndef STUDENTPTA_SCORING_H
#define STUDENTPTA_SCORING_H

#include "record.h"

// Scoring rules (design doc, section 3):
//   score = problemWeight[problem] * (timeScore + wrongScore + duplicateScore + passScore)
// Each of the four parts is worth at most 25, so a problem is out of 100 and,
// because the weights sum to 1.0, so is the whole set.
// A problem scores 0 when it was not passed or its duplicate rate is over 50%.

const int kPassScore = 25;
const int kMaxTierScore = 25;            // upper bound for any editable tier score
const int kMaxDuplicateRate = 50;        // above this the problem scores 0
const int kProblemCount = 5;             // 'A'..'E'
const double kProblemWeight[kProblemCount] = {0.10, 0.15, 0.20, 0.25, 0.30};

// Tier limits: a value v falls in tier i when v <= limits[i] (first match),
// or in the last tier when it exceeds every limit.
const int kTimeLimits[]      = {500, 1000, 3000};  // 4 tiers
const int kWrongLimits[]     = {0, 3};             // 3 tiers
const int kDuplicateLimits[] = {20, 35};           // 3 tiers (>50 is disqualified)

const int kTimeTiers      = sizeof(kTimeLimits) / sizeof(kTimeLimits[0]) + 1;
const int kWrongTiers     = sizeof(kWrongLimits) / sizeof(kWrongLimits[0]) + 1;
const int kDuplicateTiers = sizeof(kDuplicateLimits) / sizeof(kDuplicateLimits[0]) + 1;

// Points for each tier, tier 0 being the best. Editable from the menu.
struct ScoreConfig {
    int time[kTimeTiers];
    int wrong[kWrongTiers];
    int duplicate[kDuplicateTiers];
};

ScoreConfig defaultScoreConfig();

int timeTier(int seconds);
int wrongTier(int wrongTimes);
int duplicateTier(int duplicateRate);

bool isValidProblem(char problem);

// Weighted score of one record. Pure: reads only its arguments.
double scoreRecord(const Record& r, const ScoreConfig& config);

#endif
