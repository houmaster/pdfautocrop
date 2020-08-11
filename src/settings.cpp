
#include "settings.h"
#include <cstdio>

namespace {
  std::string_view unquote(std::string_view str) {
    if (str.size() >= 2 && str.front() == str.back())
      if (str.front() == '"' || str.front() == '\'')
        return str.substr(1, str.size() - 2);
    return str;
  }
} // namespace

bool interpret_commandline(Settings& settings, int argc, const char* argv[]) {
  for (auto i = 1; i < argc; i++) {
    const auto argument = std::string_view(argv[i]);
    if (argument == "-i" || argument == "--input") {
      if (++i >= argc)
        return false;
      settings.input_file = std::filesystem::u8path(unquote(argv[i]));
    }
    else if (argument == "-o" || argument == "--output") {
      if (++i >= argc)
        return false;
      settings.output_file = std::filesystem::u8path(unquote(argv[i]));
    }
    else if (argument == "-eo" || argument == "--expand-outlier") {
      settings.expand_outlier = true;
    }
    else if (argument == "-co" || argument == "--crop-outlier") {
      settings.crop_outlier = true;
    }
    else if (argument == "-d" || argument == "--outlier-dev") {
      if (++i >= argc)
        return false;
      settings.outlier_deviation = std::atof(argv[i]);
    }
    else if (argument.substr(0, 8) == "--margin") {
      if (++i >= argc)
        return false;
      const auto value = std::atof(argv[i]);
      if (argument == "--margin") {
        settings.margin_left =
        settings.margin_top =
        settings.margin_right =
        settings.margin_bottom = value;
      }
      else if (argument == "--margin-left") {
        settings.margin_left = value;
      }
      else if (argument == "--margin-top") {
        settings.margin_top = value;
      }
      else if (argument == "--margin-right") {
        settings.margin_right = value;
      }
      else if (argument == "--margin-bottom") {
        settings.margin_bottom = value;
      }
      else if (argument == "--margin-inner") {
        settings.margin_inner = value;
      }
      else if (argument == "--margin-outer") {
        settings.margin_outer = value;
      }
      else {
        return false;
      }
    }
    else if (i == argc - 1 && settings.input_file.empty()) {
      settings.input_file = std::filesystem::u8path(unquote(argv[i]));
    }
    else {
      return false;
    }
  }

  if (settings.input_file.empty())
    return false;

  if (settings.output_file.empty()) {
    auto output = settings.input_file.u8string();
    const auto dot = output.find_last_of('.');
    output.insert((dot == std::string::npos ? output.size() : dot), "-cropped");
    settings.output_file = std::filesystem::u8path(output);
  }
  return true;
}

void print_help_message(const char* argv0) {
  auto program = std::string(argv0);
  if (auto i = program.rfind('/'); i != std::string::npos)
    program = program.substr(i + 1);
  if (auto i = program.rfind('.'); i != std::string::npos)
    program = program.substr(0, i);

  const auto version =
#if __has_include("_version.h")
# include "_version.h"
  " ";
#else
  "";
#endif

  const auto defaults = Settings{ };
  std::printf(
    "autocrop %s(c) 2020 by Albert Kalchmair\n"
    "\n"
    "Usage: %s [-options] [input]\n"
    "  -i, --input <file>     input PDF filename.\n"
    "  -o, --output <file>    output PDF filename.\n"
    "  -eo, --expand-outlier  expand pages which are far smaller than average.\n"
    "  -co, --crop-outlier    crop pages which are far bigger than average.\n"
    "  -d, --outlier-dev <factor>  a page is an outlier when its size is\n"
    "      factor * standard deviation off mean page size (default: %.1f).\n"
    "  --margin <pt>          margin to add to each cropped page (default: %.0f).\n"
    "      also available: margin-left, -right, -top, -bottom, -inner, -outer\n"
    "  -h, --help             print this help.\n"
    "\n"
    "All Rights Reserved.\n"
    "This program comes with absolutely no warranty.\n"
    "See the GNU General Public License, version 3 for details.\n"
    "\n", version, program.c_str(), defaults.outlier_deviation, defaults.margin_left);
}
