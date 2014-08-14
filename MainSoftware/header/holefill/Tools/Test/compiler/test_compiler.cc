#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <OpenMesh/Core/System/compiler.hh>

#define SIZE(T) \
   std::cout << std::setw( 4) << typeid(T).name() \
             << std::setw(20) << #T << ": " << sizeof(T) << std::endl

int main(int argc, char *argv[])
{
#if defined(WIN32)
  std::cout << "          WIN32: defined\n";
#else
  std::cout << "          WIN32: undefined\n";
#endif
#if defined(OM_CC_MSVC)
  std::cout << "     OM_CC_MSVC: exists\n";
#endif
#if defined(OM_CC_ICC)
  std::cout << "      OM_CC_ICC: exists\n";
#endif
#if defined(OM_CC_GCC)
  std::cout << "      OM_CC_GCC: exists\n";
#endif
#if defined(OM_CC_MIPS)
  std::cout << "     OM_CC_MIPS: exists\n";
#endif
  std::cout << "          OM_CC: " << OM_CC << std::endl;
  std::cout << "  OM_CC_VERSION: " << OM_CC_VERSION << std::endl;
#if defined(OM_STATIC_BUILD)
  std::cout << "OM_STATIC_BUILD: " << OM_STATIC_BUILD << std::endl;
#else
  std::cout << "OM_STATIC_BUILD: not defined" << std::endl;
#endif

  std::cout << std::endl << "Template settings:\n";
  std::cout << "        OM_OUT_OF_CLASS_TEMPLATE: " << OM_OUT_OF_CLASS_TEMPLATE << std::endl;
  std::cout << "       OM_PARTIAL_SPECIALIZATION: " << OM_PARTIAL_SPECIALIZATION << std::endl;
  std::cout << "            OM_INCLUDE_TEMPLATES: " << OM_INCLUDE_TEMPLATES << std::endl;
  std::cout << std::endl;

  std::cout << "Size of fundamental types:\n";
  SIZE(bool);
  SIZE(char);
  SIZE(unsigned char);
  SIZE(short);
  SIZE(unsigned short);
  SIZE(int);
  SIZE(unsigned int);
  SIZE(long);
  SIZE(unsigned long);
#if defined(OM_CC_MSVC)
  SIZE(__int64);
  SIZE(unsigned __int64);
#else
  SIZE(long long);
  SIZE(unsigned long long);
#endif
  SIZE(float);
  SIZE(double);
  SIZE(long double);

  std::cout << std::endl;

  return 0;
}
