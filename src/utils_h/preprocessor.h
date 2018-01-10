#pragma once

#define UTILS_H_PRIV_NUMBER_TO_STR_HELPER(x) #x
#define UTILS_H_NUMBER_TO_STR(x) UTILS_H_PRIV_NUMBER_TO_STR_HELPER(x)

#define UTILS_H_PRIV_NUMBER_TO_WSTR_HELPER(x) L#x
#define UTILS_H_NUMBER_TO_WSTR(x) UTILS_H_PRIV_NUMBER_TO_WSTR_HELPER(x)

#define UTILS_H_PRIV_STR_TO_WSTR_HELPER(x) L##x
#define UTILS_H_STR_TO_WSTR(x) UTILS_H_PRIV_STR_TO_WSTR_HELPER(x)

#define UTILS_H_PRIV_CONCATENATE_HELPER(x, y) x##y
#define UTILS_H_CONCATENATE(x, y) UTILS_H_PRIV_CONCATENATE_HELPER(x, y)

#define UTILS_H_LINE_STR UTILS_H_NUMBER_TO_STR(__LINE__)
#define UTILS_H_LINE_WSTR UTILS_H_NUMBER_TO_WSTR(__LINE__)
#define UTILS_H_FILE_WSTR UTILS_H_STR_TO_WSTR(__FILE__)
#define UTILS_H_FUNC_WSTR UTILS_H_STR_TO_WSTR(__FUNCTION__)

#define UTILS_H_UNIQUE_NAME UTILS_H_CONCATENATE(utilsUniqueName, __COUNTER__)
