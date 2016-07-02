#pragma once

#include "Current/TypeSystem/struct.h"

CURRENT_STRUCT(Card) {
  CURRENT_FIELD(color, size_t);
  CURRENT_FIELD(points, size_t);
  CURRENT_FIELD(price, std::vector<size_t>);  // A vector of size `box.colors.size()`.
};

CURRENT_STRUCT(Deck) {
  CURRENT_FIELD(tier, std::vector<std::vector<Card>>);  // { [ Tier `i` cards ] ... }.
};

CURRENT_STRUCT(Noble) {
  CURRENT_FIELD(points, size_t);
  CURRENT_FIELD(requirements, std::vector<size_t>);  // A vector of size `box.colors.size()`.
};

CURRENT_STRUCT(Box) {
  CURRENT_FIELD(tiers, size_t);  // The number of tiers in play.
  CURRENT_FIELD(deck, Deck);
  CURRENT_FIELD(nobles, std::vector<Noble>);
  CURRENT_FIELD(colors, std::vector<std::string>);  // The names of colors in play, primarily for UX purposes.
};
