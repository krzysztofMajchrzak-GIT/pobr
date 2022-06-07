#include <optional>
#include <vector>

#include "FielndInvariants.h"

struct PotentialHit
{
    std::vector<FieldInvariants> outlyingLetters;
    std::optional<FieldInvariants> secondLetterTop;
    std::optional<FieldInvariants> secondLetterBottom;
    std::optional<FieldInvariants> thirdLetter;
    std::optional<FieldInvariants> background;
    bool checkHit();
    PotentialHit();
};
