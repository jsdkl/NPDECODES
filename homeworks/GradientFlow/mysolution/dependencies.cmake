# Add your custom dependencies here:

# DIR will be provided by the calling file.

set(SOURCES
  ${DIR}/gradientflow_main.cc
  ${DIR}/gradientflow.h
  ${DIR}/gradientflow.cc
)

set(LIBRARIES
  Eigen3::Eigen
)
