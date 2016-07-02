#include "schema_box.h"

#include "Current/Bricks/dflags/dflags.h"
#include "Current/Bricks/file/file.h"
#include "Current/Bricks/strings/join.h"
#include "Current/Bricks/strings/split.h"

#include "Current/TypeSystem/Serialization/json.h"

DEFINE_string(input, "box.txt", "The input, CSV, set of cards and nobles in the game.");
DEFINE_string(output, "box.json", "The output rules of the game in a structured format.");

// The name of the five colors of five resources used in the game.
static const std::vector<std::string> colors = {"Blue", "White", "Black", "Green", "Red"};

template <typename T>
inline std::vector<size_t> ParsePrice(T begin, T end, size_t N) {
  std::vector<size_t> result;
  for (; begin != end; ++begin) {
    result.push_back(current::strings::FromString<size_t>(*begin));
  }
  assert(result.size() == N);
  return result;
}

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  std::map<std::string, size_t> name2index;
  for (size_t i = 0; i < colors.size(); ++i) {
    name2index[colors[i]] = i;
  }
  assert(name2index.size() == colors.size());

  Box box;
  box.colors = colors;
  box.tiers = 3;  // Number of tiers.
  box.deck.tier.resize(box.tiers);

  for (const std::string& line :
       current::strings::Split<current::strings::ByLines>(current::FileSystem::ReadFileAsString(FLAGS_input))) {
    const std::vector<std::string> fields = current::strings::Split(line, ',');
    assert(fields.size() == 9);
    if (fields[0] == "Legend:Type") {
      assert(line == "Legend:Type,Name,Bonus,Points," + current::strings::Join(colors, ','));
    } else if (fields[0] == "1" || fields[0] == "2" || fields[0] == "3") {
      assert(name2index.count(fields[2]));
      Card card;
      card.color = name2index[fields[2]];
      card.points = current::FromString<size_t>(fields[3]);
      card.price = ParsePrice(fields.begin() + 4, fields.end(), box.colors.size());
      box.deck.tier[fields[0][0] - '1'].push_back(card);
    } else {
      assert(fields[0] == "N");
      Noble noble;
      noble.points = current::FromString<size_t>(fields[3]);
      noble.requirements = ParsePrice(fields.begin() + 4, fields.end(), box.colors.size());
      box.nobles.push_back(noble);
    }
  }

  current::FileSystem::WriteStringToFile(JSON(box), FLAGS_output.c_str());
}
