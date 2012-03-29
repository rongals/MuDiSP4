FILE(REMOVE_RECURSE
  "CMakeFiles/doxygen_docs"
  "doxygen"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/doxygen_docs.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
