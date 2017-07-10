#include "Utilities/Testing/interface/CppUnit_testdriver.icpp"
#include "cppunit/extensions/HelperMacros.h"
#include "DetectorDescription/Core/interface/Singleton.h"
#include "DetectorDescription/Core/interface/Singleton.icc"

class Dummy
{
public:
  Dummy( )
    : value( 100. )
    {}
  
  double value;
};

template class DDI::Singleton<Dummy>;

class testSingleton : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( testSingleton );
  CPPUNIT_TEST( testEquality );
  
  CPPUNIT_TEST_SUITE_END();
  
public:

  void setUp( ) 
    {
      m_s = &DDI::Singleton<Dummy>::instance();
    }
  
  void testEquality( );

private:
  
  Dummy *m_s;  
  Dummy *m_copy;  
};

void
testSingleton::testEquality( )
{
  m_s = &DDI::Singleton<Dummy>::instance();
  m_copy = &DDI::Singleton<Dummy>::instance();

  CPPUNIT_ASSERT( m_s != 0 );
  CPPUNIT_ASSERT( m_copy != 0 );
  CPPUNIT_ASSERT( m_s == m_copy );
  CPPUNIT_ASSERT( m_s->value == m_copy->value );
  m_s->value = 50.0;
  CPPUNIT_ASSERT( m_s->value == m_copy->value );
}

CPPUNIT_TEST_SUITE_REGISTRATION( testSingleton );
