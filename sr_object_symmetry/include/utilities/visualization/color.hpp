// Copyright 2017 Aleksandrs Ecins
// Licensed under GPLv2+
// Refer to the LICENSE.txt file included.

#ifndef VISUALIZATION_COLOR_HPP_
#define VISUALIZATION_COLOR_HPP_



// PCL
#include <pcl/io/point_cloud_image_extractors.h>
// CPP tools
#include <utilities/visualization/color.h>

#ifdef PCL_COMMON_COLORS_H
#include <pcl/common/colors.h>
#else
#include <pcl/io/impl/point_cloud_image_extractors.hpp>
#endif

// STD
#include <iostream>
#include <vector>
#include <algorithm>

namespace utl
{
//--------------------------------------------------------------------------
// Color generators
//--------------------------------------------------------------------------

/** \brief Generate a random color
    *  \return color      RGB color triplet corresponding to the element. Colors are in [0,1] range
    */
inline Color getRandomColor()
{
  Color color;
  std::cout << "[utl::pclvis::getRandomColor] not implemented yet!" << std::endl;
  std::abort();

  return color;
}

/** \brief Generate a random color
    *  \param[in]  id      index of the element
    *  \return color      RGB color triplet corresponding to the element. Colors are in [0,1] range
    */
inline Color getGlasbeyColor(const int id)
{
  Color color;

// IF PCL 1.8 trunk
#ifdef PCL_COMMON_COLORS_H
  int tmpId = id % pcl::GlasbeyLUT().size();
  pcl::RGB color_pcl = pcl::GlasbeyLUT().at(tmpId);
  color.r = static_cast<float>(color_pcl.r) / 255;
  color.g = static_cast<float>(color_pcl.g) / 255;
  color.b = static_cast<float>(color_pcl.b) / 255;
#else
  const uint8_t r = rand() % 255;
  const uint8_t g = rand() % 255;
  const uint8_t b = rand() % 255;
  color.r = r;
  color.g = g;
  color.b = b;
#endif

  return color;
}

/** \brief Convert a single point with RGB information to grayscale
    *  \param[in,out]  point point to be converted to grayscale
    *  \note conversion is done using the formula used in OpenCV
 * (http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html)
    */
template <typename PointT>
inline void rgb2gray(PointT& point)
{
  uint8_t gray = static_cast<uint8_t>(static_cast<float>(point.r) * 0.299 + static_cast<float>(point.r) * 0.587 +
                                      static_cast<float>(point.r) * 0.114);
  point.r = gray;
  point.g = gray;
  point.b = gray;
}

/** \brief Convert color in pointcloud to grayscale
    *  \param[in,out]  cloud cloud to be converted to grayscale
    *  \note conversion is done using the formula used in OpenCV
 * (http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html)
    */
template <typename PointT>
inline void rgb2gray(pcl::PointCloud<PointT>& cloud)
{
  for (size_t pointId = 0; pointId < cloud.size(); pointId++)
    rgb2gray<PointT>(cloud.points[pointId]);
}

/** \brief Change the tint of a point to a given color
    *  \param[in,out]  point point to be converted to grayscale
    *  \param[in]      color color of the tint
    *  \param[in]      alpha amount of tint. 0 leaves point color unchanged while 1 replace it by input color. Default
 * 0.5
    */
template <typename PointT>
inline void tintPoint(PointT& point, const Color& color, const float alpha = 0.3)
{
  point.r =
      static_cast<uint8_t>(std::min(255.0f, static_cast<float>(point.r * (1 - alpha) + color.r * alpha * 255.0f)));
  point.g =
      static_cast<uint8_t>(std::min(255.0f, static_cast<float>(point.g * (1 - alpha) + color.g * alpha * 255.0f)));
  point.b =
      static_cast<uint8_t>(std::min(255.0f, static_cast<float>(point.b * (1 - alpha) + color.b * alpha * 255.0f)));
}

/** \brief Change the tint of a pointcloud to a given color
    *  \param[in,out]  cloud pointcloud to be converted to grayscale
    *  \param[in]      color color of the tint
    *  \param[in]      alpha amount of tint. 0 leaves point color unchanged while 1 replace it by input color. Default
 * 0.5
    */
template <typename PointT>
inline void tintPointCloud(pcl::PointCloud<PointT>& cloud, const std::vector<int>& indices, const Color& color,
                           const float alpha = 0.3)
{
  for (size_t pointIdIt = 0; pointIdIt < indices.size(); pointIdIt++)
    tintPoint<PointT>(cloud.points[indices[pointIdIt]], color, alpha);
}

/** \brief Change the tint of a pointcloud to a given color
    *  \param[in,out]  cloud pointcloud to be converted to grayscale
    *  \param[in]      color color of the tint
    *  \param[in]      alpha amount of tint. 0 leaves point color unchanged while 1 replace it by input color. Default
 * 0.5
    */
template <typename PointT>
inline void tintPointCloud(pcl::PointCloud<PointT>& cloud, const Color& color, const float alpha = 0.3)
{
  std::vector<int> indices(cloud.size());
  for (size_t pointId = 0; pointId < cloud.size(); pointId++)
    indices[pointId] = pointId;

  tintPointCloud<PointT>(cloud, indices, color, alpha);
}
}  // namespace utl

#endif  // VISUALIZATION_COLOR_HPP_
