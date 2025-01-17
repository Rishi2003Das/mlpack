/**
 * @file bindings/R/get_printable_type_impl.hpp
 * @author Yashwant Singh Parihar
 *
 * Template metaprogramming to return the string representation of the R
 * type for a given R binding parameter.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_R_GET_PRINTABLE_TYPE_IMPL_HPP
#define MLPACK_BINDINGS_R_GET_PRINTABLE_TYPE_IMPL_HPP

#include "get_printable_type.hpp"

namespace mlpack {
namespace bindings {
namespace r {

template<typename T>
inline std::string GetPrintableType(
    util::ParamData& /* d */,
    const std::enable_if_t<!util::IsStdVector<T>::value>*,
    const std::enable_if_t<!data::HasSerialize<T>::value>*,
    const std::enable_if_t<!arma::is_arma_type<T>::value>*,
    const std::enable_if_t<!std::is_same_v<T,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "unknown";
}

template<>
inline std::string GetPrintableType<int>(
    util::ParamData& /* d */,
    const std::enable_if_t<!util::IsStdVector<int>::value>*,
    const std::enable_if_t<!data::HasSerialize<int>::value>*,
    const std::enable_if_t<!arma::is_arma_type<int>::value>*,
    const std::enable_if_t<!std::is_same_v<int,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "integer";
}

template<>
inline std::string GetPrintableType<double>(
    util::ParamData& /* d */,
    const std::enable_if_t<!util::IsStdVector<double>::value>*,
    const std::enable_if_t<!data::HasSerialize<double>::value>*,
    const std::enable_if_t<!arma::is_arma_type<double>::value>*,
    const std::enable_if_t<!std::is_same_v<double,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "numeric";
}

template<>
inline std::string GetPrintableType<std::string>(
    util::ParamData& /* d */,
    const std::enable_if_t<!util::IsStdVector<std::string>::value>*,
    const std::enable_if_t<!data::HasSerialize<std::string>::value>*,
    const std::enable_if_t<!arma::is_arma_type<std::string>::value>*,
    const std::enable_if_t<
        !std::is_same_v<std::string,
         std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "character";
}

template<>
inline std::string GetPrintableType<size_t>(
    util::ParamData& /* d */,
    const std::enable_if_t<!util::IsStdVector<size_t>::value>*,
    const std::enable_if_t<!data::HasSerialize<size_t>::value>*,
    const std::enable_if_t<!arma::is_arma_type<size_t>::value>*,
    const std::enable_if_t<!std::is_same_v<size_t,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "integer";
}

template<>
inline std::string GetPrintableType<bool>(
    util::ParamData& /* d */,
    const std::enable_if_t<!util::IsStdVector<bool>::value>*,
    const std::enable_if_t<!data::HasSerialize<bool>::value>*,
    const std::enable_if_t<!arma::is_arma_type<bool>::value>*,
    const std::enable_if_t<!std::is_same_v<bool,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "logical";
}

template<typename T>
inline std::string GetPrintableType(
    util::ParamData& d,
    const std::enable_if_t<util::IsStdVector<T>::value>*,
    const std::enable_if_t<!std::is_same_v<T,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "vector of " + GetPrintableType<typename T::value_type>(d) + "s";
}

template<typename T>
inline std::string GetPrintableType(
    util::ParamData& /* d */,
    const std::enable_if_t<arma::is_arma_type<T>::value>*,
    const std::enable_if_t<!std::is_same_v<T,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  std::string type = "numeric matrix";
  if (std::is_same_v<typename T::elem_type, double>)
  {
    if (T::is_row || T::is_col)
      type = "numeric vector";
  }
  else if (std::is_same_v<typename T::elem_type, size_t>)
  {
    type = "integer matrix";
    if (T::is_row || T::is_col)
      type = "integer vector";
  }

  return type;
}

template<typename T>
inline std::string GetPrintableType(
    util::ParamData& /* d */,
    const std::enable_if_t<std::is_same_v<T,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  return "categorical matrix/data.frame";
}

template<typename T>
inline std::string GetPrintableType(
    util::ParamData& d,
    const std::enable_if_t<!arma::is_arma_type<T>::value>*,
    const std::enable_if_t<data::HasSerialize<T>::value>*,
    const std::enable_if_t<!std::is_same_v<T,
        std::tuple<data::DatasetInfo, arma::mat>>>*)
{
  std::string type = util::StripType(d.cppType);
  if (type == "mlpackModel")
  {
    // If this is true, then we are being called from the Markdown bindings.
    // This will be printed as the general documentation for model types.
    return "<Model> (mlpack model)";
  }
  else
  {
    return type;
  }
}

} // namespace r
} // namespace bindings
} // namespace mlpack

#endif
