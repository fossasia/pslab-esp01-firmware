if ! which clang-format &>/dev/null; then
  echo "clang-format not found or not in PATH. Please install: https://github.com/arduino/clang-static-binaries/releases"
  exit 1
fi

find \
  src \
  \( \
    -name '*.c' -or \
    -name '*.cpp' -or \
    -name '*.h' -or \
    -name '*.ino' -or \
    -name '*.ipp' -or \
    -name '*.tpp' \
  \) \
  -type f \
  -exec \
    clang-format \
      --assume-filename=foo.cpp \
      -i \
      --style=file \
      {} \;
