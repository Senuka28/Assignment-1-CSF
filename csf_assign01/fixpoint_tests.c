#include <stdlib.h>
#include <string.h>
#include "tctest.h"
#include "fixpoint.h"

// Test fixture: defines some fixpoint_t instances
// that can be used by test functions
typedef struct {
  fixpoint_t zero;
  fixpoint_t one;
  fixpoint_t one_half;
  fixpoint_t max;
  fixpoint_t neg_three_eighths;
  fixpoint_t min;
  fixpoint_t one_and_one_half;
  fixpoint_t one_hundred;
  fixpoint_t neg_eleven;
} TestObjs;

// Functions to create and destroy the text fixture
// (each test function gets a "fresh" instance of the
// test fixture)
TestObjs *setup( void );
void cleanup( TestObjs *objs );

// The setup() function uses this macro to initialize
// the fixpoint_t objects in the test fixture, to avoid
// being dependent on the fixpoint_init() function.
#define TEST_FIXPOINT_INIT( val, w, f, n ) \
do { \
  (val)->frac = f; \
  (val)->whole = w; \
  (val)->negative = n; \
} while ( 0 )

// Macro to check two fixpoint_t instances for exact equality
#define TEST_EQUAL( val1, val2 ) \
do { \
  ASSERT( (val1)->whole == (val2)->whole ); \
  ASSERT( (val1)->frac == (val2)->frac ); \
  ASSERT( (val1)->negative == (val2)->negative ); \
} while ( 0 )

// Convenience macro to turn a string literal into a const pointer
// to a temporary instance of fixpoint_str_t
#define FIXPOINT_STR( strlit ) &( ( fixpoint_str_t ) { .str = (strlit) } )

// Prototypes for test functions
void test_init( TestObjs *objs );
void test_get_whole( TestObjs *objs );
void test_get_frac( TestObjs *objs );
void test_is_negative( TestObjs *objs );
void test_negate( TestObjs *objs );
void test_add( TestObjs *objs );
void test_sub( TestObjs *objs );
void test_mul( TestObjs *objs );
void test_compare( TestObjs *objs );
void test_format_hex( TestObjs *objs );
void test_parse_hex( TestObjs *objs );
// TODO: add additional test functions

int main( int argc, char **argv ) {
  if ( argc > 1 )
    tctest_testname_to_execute = argv[1];

  TEST_INIT();

  TEST( test_init );
  TEST( test_get_whole );
  TEST( test_get_frac );
  TEST( test_is_negative );
  TEST( test_negate );
  TEST( test_add );
  TEST( test_sub );
  TEST( test_mul );
  TEST( test_compare );
  TEST( test_format_hex );
  TEST( test_parse_hex );
  // TODO: call additional test functions

  TEST_FINI();
}

TestObjs *setup( void ) {
  TestObjs *objs = (TestObjs *) malloc( sizeof( TestObjs ) );

  TEST_FIXPOINT_INIT( &objs->zero, 0, 0, false );
  TEST_FIXPOINT_INIT( &objs->one, 1, 0, false );
  TEST_FIXPOINT_INIT( &objs->one_half, 0, 0x80000000, false );
  TEST_FIXPOINT_INIT( &objs->max, 0xFFFFFFFF, 0xFFFFFFFF, false );
  TEST_FIXPOINT_INIT( &objs->neg_three_eighths, 0, 0x60000000, true );
  TEST_FIXPOINT_INIT( &objs->min, 0, 1, false );
  TEST_FIXPOINT_INIT( &objs->one_and_one_half, 1, 0x80000000, false );
  TEST_FIXPOINT_INIT( &objs->one_hundred, 100, 0, false );
  TEST_FIXPOINT_INIT( &objs->neg_eleven, 11, 0, true );

  // TODO: initialize additional fixpoint_t instances

  return objs;
}

void cleanup( TestObjs *objs ) {
  free( objs );
}

void test_init( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_t val;

  fixpoint_init( &val, 0, 0, false );
  ASSERT( val.whole == 0 );
  ASSERT( val.frac == 0 );
  ASSERT( val.negative == false );

  fixpoint_init( &val, 0xad2b55b1, 0xcf5f4470, true );
  ASSERT( val.whole == 0xad2b55b1 );
  ASSERT( val.frac == 0xcf5f4470 );
  ASSERT( val.negative == true );
}

void test_get_whole( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  ASSERT( fixpoint_get_whole( &objs->zero ) == 0 );
  ASSERT( fixpoint_get_whole( &objs->one ) == 1 );
  ASSERT( fixpoint_get_whole( &objs->one_half ) == 0 );
  ASSERT( fixpoint_get_whole( &objs->max ) == 0xFFFFFFFF );
  ASSERT( fixpoint_get_whole( &objs->neg_three_eighths ) == 0 );
  ASSERT( fixpoint_get_whole( &objs->min ) == 0 );
  ASSERT( fixpoint_get_whole( &objs->one_and_one_half ) == 1 );
  ASSERT( fixpoint_get_whole( &objs->one_hundred ) == 100 );
  ASSERT( fixpoint_get_whole( &objs->neg_eleven ) == 11 );
}

void test_get_frac( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  ASSERT( fixpoint_get_frac( &objs->zero ) == 0 );
  ASSERT( fixpoint_get_frac( &objs->one ) == 0 );
  ASSERT( fixpoint_get_frac( &objs->one_half ) == 0x80000000 );
  ASSERT( fixpoint_get_frac( &objs->max ) == 0xFFFFFFFF );
  ASSERT( fixpoint_get_frac( &objs->neg_three_eighths ) == 0x60000000 );
  ASSERT( fixpoint_get_frac( &objs->min ) == 1 );
  ASSERT( fixpoint_get_frac( &objs->one_and_one_half ) == 0x80000000 );
  ASSERT( fixpoint_get_frac( &objs->one_hundred ) == 0 );
  ASSERT( fixpoint_get_frac( &objs->neg_eleven ) == 0 );
}

void test_is_negative( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  ASSERT( fixpoint_is_negative( &objs->zero ) == false );
  ASSERT( fixpoint_is_negative( &objs->one ) == false );
  ASSERT( fixpoint_is_negative( &objs->one_half ) == false );
  ASSERT( fixpoint_is_negative( &objs->max ) == false );
  ASSERT( fixpoint_is_negative( &objs->neg_three_eighths ) == true );
  ASSERT( fixpoint_is_negative( &objs->min ) == false );
  ASSERT( fixpoint_is_negative( &objs->one_and_one_half ) == false );
  ASSERT( fixpoint_is_negative( &objs->one_hundred ) == false );
  ASSERT( fixpoint_is_negative( &objs->neg_eleven ) == true );
}

void test_negate( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_t result;

  // Negating 0 shouldn't cause it to become negative
  result = objs->zero;
  ASSERT( false == result.negative );
  fixpoint_negate( &result );
  ASSERT( result.whole == objs->zero.whole );
  ASSERT( result.frac == objs->zero.frac );
  ASSERT( false == result.negative );

  // Non-zero values should have their sign flip when negated,
  // but the magnitude should stay the same

  result = objs->one;
  fixpoint_negate( &result );
  ASSERT( result.whole == objs->one.whole );
  ASSERT( result.frac == objs->one.frac );
  ASSERT( true == result.negative );

  result = objs->max;
  fixpoint_negate( &result );
  ASSERT( result.whole == objs->max.whole );
  ASSERT( result.frac == objs->max.frac );
  ASSERT( true == result.negative );

  result = objs->neg_three_eighths;
  fixpoint_negate( &result );
  ASSERT( result.whole == objs->neg_three_eighths.whole );
  ASSERT( result.frac == objs->neg_three_eighths.frac );
  ASSERT( false == result.negative );
}

void test_add( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_t result;

  ASSERT( fixpoint_add( &result, &objs->zero, &objs->zero ) == RESULT_OK );
  ASSERT( 0 == result.frac );
  ASSERT( 0 == result.whole );
  ASSERT( false == result.negative );

  ASSERT( fixpoint_add( &result, &objs->zero, &objs->one ) == RESULT_OK );
  ASSERT( 0 == result.frac );
  ASSERT( 1 == result.whole );
  ASSERT( false == result.negative );

  ASSERT( fixpoint_add( &result, &objs->max, &objs->one ) == RESULT_OVERFLOW );

  ASSERT( fixpoint_add( &result, &objs->max, &objs->min ) == RESULT_OVERFLOW );

  ASSERT( fixpoint_add( &result, &objs->zero, &objs->neg_three_eighths ) == RESULT_OK );
  ASSERT( 0x60000000 == result.frac );
  ASSERT( 0 == result.whole );
  ASSERT( true == result.negative );

  fixpoint_t neg_max = objs->max;
  neg_max.negative = true;

  fixpoint_t neg_min = objs->min;
  neg_min.negative = true;

  ASSERT( fixpoint_add( &result, &neg_max, &neg_min ) == RESULT_OVERFLOW );
}

void test_sub( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_t result;

  ASSERT( fixpoint_sub( &result, &objs->one, &objs->zero ) == RESULT_OK );
  ASSERT( 1 == result.whole );
  ASSERT( 0 == result.frac );
  ASSERT( false == result.negative );

  ASSERT( fixpoint_sub( &result, &objs->zero, &objs->one ) == RESULT_OK );
  ASSERT( 1 == result.whole );
  ASSERT( 0 == result.frac );
  ASSERT( true == result.negative );

  fixpoint_t neg_min = objs->min;
  fixpoint_negate( &neg_min );
  ASSERT( fixpoint_sub( &result, &neg_min, &objs->max ) == RESULT_OVERFLOW );
}

void test_mul( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_t result;

  ASSERT( fixpoint_mul( &result, &objs->one, &objs->zero ) == RESULT_OK );
  ASSERT( 0 == result.whole );
  ASSERT( 0 == result.frac );
  ASSERT( false == result.negative );

  ASSERT( fixpoint_mul( &result, &objs->one_and_one_half, &objs->one_hundred ) == RESULT_OK );
  ASSERT( 150 == result.whole );
  ASSERT( 0 == result.frac );
  ASSERT( false == result.negative );
}

void test_compare( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  ASSERT( 0 == fixpoint_compare( &objs->zero, &objs->zero ) );
  ASSERT( 1 == fixpoint_compare( &objs->one, &objs->zero ) );
  ASSERT( -1 == fixpoint_compare( &objs->zero, &objs->one ) );
  ASSERT( -1 == fixpoint_compare( &objs->neg_three_eighths, &objs->one_half ) );
  ASSERT( 1 == fixpoint_compare( &objs->one_half, &objs->neg_three_eighths ) );
}

void test_format_hex( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_str_t s;

  fixpoint_format_hex( &s, &objs->zero );
  ASSERT( 0 == strcmp( "0.0", s.str ) );

  fixpoint_format_hex( &s, &objs->one );
  ASSERT( 0 == strcmp( "1.0", s.str ) );

  fixpoint_format_hex( &s, &objs->one_half );
  ASSERT( 0 == strcmp( "0.8", s.str ) );

  fixpoint_format_hex( &s, &objs->max );
  ASSERT( 0 == strcmp( "ffffffff.ffffffff", s.str ) );

  fixpoint_format_hex( &s, &objs->neg_three_eighths );
  ASSERT( 0 == strcmp( "-0.6", s.str ) );

  fixpoint_format_hex( &s, &objs->min );
  ASSERT( 0 == strcmp( "0.00000001", s.str ) );

  fixpoint_format_hex( &s, &objs->one_and_one_half );
  ASSERT( 0 == strcmp( "1.8", s.str ) );

  fixpoint_format_hex( &s, &objs->one_hundred );
  ASSERT( 0 == strcmp( "64.0", s.str ) );

  fixpoint_format_hex( &s, &objs->neg_eleven );
  ASSERT( 0 == strcmp( "-b.0", s.str ) );
}

void test_parse_hex( TestObjs *objs ) {
  // Note: don't modify the provided test functions.
  // Instead, add new test functions containing your new tests.

  fixpoint_t val;

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "0.0" ) ) );
  TEST_EQUAL( &objs->zero, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "1.0" ) ) );
  TEST_EQUAL( &objs->one, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "0.8" ) ) );
  TEST_EQUAL( &objs->one_half, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "ffffffff.ffffffff" ) ) );
  TEST_EQUAL( &objs->max, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "-0.6" ) ) );
  TEST_EQUAL( &objs->neg_three_eighths, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "0.00000001" ) ) );
  TEST_EQUAL( &objs->min, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "1.8" ) ) );
  TEST_EQUAL( &objs->one_and_one_half, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "1.8" ) ) );
  TEST_EQUAL( &objs->one_and_one_half, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "64.0" ) ) );
  TEST_EQUAL( &objs->one_hundred, &val );

  ASSERT( true == fixpoint_parse_hex( &val, FIXPOINT_STR( "-b.0" ) ) );
  TEST_EQUAL( &objs->neg_eleven, &val );

  // Note: this test function doesn't have any tests for invalid
  // hex strings. You should add tests for invalid strings in one of
  // your own test functions.
}

// TODO: define additional test functions

void test_another_init( TestObjs *objs ) {
  fixpoint_t val;

  // Test 1: Zero with negative true (should force false)
  fixpoint_init( &val, 0, 0, true );
  ASSERT( val.whole == 0 );
  ASSERT( val.frac == 0 );
  ASSERT( val.negative == false );

  // Test 2: Maximum values
  fixpoint_init( &val, 0xFFFFFFFF, 0xFFFFFFFF, false );
  ASSERT( val.whole == 0xFFFFFFFF );
  ASSERT( val.frac == 0xFFFFFFFF );
  ASSERT( val.negative == false );


}

void test_another_get_whole( TestObjs *objs ) {
  // Test 1: Maximum whole value
  fixpoint_t max_whole;
  fixpoint_init( &max_whole, 0xFFFFFFFF, 0x80000000, false );
  ASSERT( fixpoint_get_whole( &max_whole ) == 0xFFFFFFFF );

  // Test 2: Minimum whole
  fixpoint_t min_whole;
  fixpoint_init( &min_whole, 1, 0x80000000, false );
  ASSERT( fixpoint_get_whole( &min_whole ) == 1 );
}

void test_another_get_frac( TestObjs *objs) {
  // Test 1: Maximum fraction value
  fixpoint_t max_frac;
  fixpoint_init( &max_frac, 0, 0xFFFFFFFF, false );
  ASSERT( fixpoint_get_frac( &max_frac ) == 0xFFFFFFFF );

  // Test 2: Minimum fraction value (1)
  fixpoint_t min_frac;
  fixpoint_init( &min_frac, 0, 1, false );
  ASSERT( fixpoint_get_frac( &min_frac ) == 1 );
}

void test_another_is_negative( TestObjs *objs) {
  fixpoint_t val;
  
  // Test 1: Zero should never be negative
  fixpoint_init( &val, 0, 0, true );
  ASSERT( fixpoint_is_negative( &val ) == false );

  // Test 2: Maximum positive value
  fixpoint_init( &val, 0xFFFFFFFF, 0xFFFFFFFF, false );
  ASSERT( fixpoint_is_negative( &val ) == false );

  // Test 3: Maximum negative value
  fixpoint_init( &val, 0xFFFFFFFF, 0xFFFFFFFF, true );
  ASSERT( fixpoint_is_negative( &val ) == true );
}

void test_another_negate( TestObjs *objs) {
  fixpoint_t result;
  // Test 1: Maximum positive value
  result = objs->max;
  fixpoint_negate( &result );
  ASSERT( result.whole == objs->max.whole );
  ASSERT( result.frac == objs->max.frac );
  ASSERT( true == result.negative );

  // Test 2: Maximum negative value (should become positive)
  fixpoint_t max_neg;
  fixpoint_init( &max_neg, 0xFFFFFFFF, 0xFFFFFFFF, true );
  fixpoint_negate( &max_neg );
  ASSERT( max_neg.whole == 0xFFFFFFFF );
  ASSERT( max_neg.frac == 0xFFFFFFFF );
  ASSERT( false == max_neg.negative );
  
}

void test_another_add( TestObjs *objs ) {
  fixpoint_t aggregate;

  // addition with borrow that invokes subtraction-like case 1.0 + -0.00000001
  fixpoint_t one = objs->one;
  fixpoint_t negative_min = objs->min;
  negative_min.negative = true;

  ASSERT(fixpoint_add(&aggregate, &one, &negative_min) == RESULT_OK);
  ASSERT(aggregate.whole == 0);
  ASSERT(aggregate.frac == 0xFFFFFFFF);
  ASSERT(aggregate.negative == false);

  // test for cases of same magnitude but opposite sign --> 0
  fixpoint_t negative_one = objs->one;
  negative_one.negative = true;
  ASSERT(fixpoint_add(&aggregate, &objs->one, &negative_one) == RESULT_OK);
  ASSERT(aggregate.frac == 0 && aggregate.whole == 0 && aggregate.negative == false); // check if 0 here
}

void test_another_sub( TestObjs *objs ) {
  fixpoint_t aggregate;

  // subtraction with a borrowing, case is 1.0 – 0.00000001
  ASSERT(fixpoint_sub(&aggregate, &objs->one, &objs->min) == RESULT_OK);
  ASSERT(aggregate.whole == 0);
  ASSERT(aggregate.frac == 0xFFFFFFFF);
  ASSERT(aggregate.negative == false);

  // negative result case next of 0 - 1.5
  ASSERT(fixpoint_sub(&aggregate, &objs->zero, &objs->one_and_one_half) == RESULT_OK);
  ASSERT(aggregate.whole == 1);
  ASSERT(aggregate.frac == 0x80000000);
  ASSERT(aggregate.negative == true);
}

void test_another_mul( TestObjs *objs ) {
  fixpoint_t aggregate;

  // start with fraction times fraction case 0.5 * 0.5 = 0.25
  ASSERT(fixpoint_mul(&aggregate, &objs->one_half, &objs->one_half) == RESULT_OK);
  ASSERT(aggregate.frac == 0x40000000);
  ASSERT(aggregate.whole == 0);

  // next try out negative and positive mul
  ASSERT(fixpoint_mul(&aggregate, &objs->neg_eleven, &objs->one) == RESULT_OK);
  ASSERT(aggregate.frac == 0);
  ASSERT(aggregate.whole == 11);
  ASSERT(aggregate.negative == true);

  // check for overflow here as case max * 2
  fixpoint_t two;
  TEST_FIXPOINT_INIT(&two, 2, 0, false);
  ASSERT(fixpoint_mul(&aggregate, &objs->max, &two) & RESULT_OVERFLOW);

  // last and comparatively check for underflw as case 0.00000001 × 0.00000001
  fixpoint_t tiny = objs->min;
  ASSERT(fixpoint_mul(&aggregate, &tiny, &tiny) & RESULT_UNDERFLOW);
}

void test_another_compare( TestObjs *objs ) {
  fixpoint_t a;
  fixpoint_t b;

  TEST_FIXPOINT_INIT(&a, 1, 0x40000000, false); // this is 1.25
  TEST_FIXPOINT_INIT(&b, 1, 0x20000000, false); // and this is 1.125
  
  ASSERT(fixpoint_compare(&a, &b) == 1);
  ASSERT(fixpoint_compare(&b, &a) == -1);

  // then test negative vs positive
  ASSERT(fixpoint_compare(&objs->neg_eleven, &objs->one) == -1);

  // check equal negatives next
  fixpoint_t neg_eleven2 = objs->neg_eleven;
  ASSERT(fixpoint_compare(&objs->neg_eleven, &neg_eleven2) == 0);
}

void test_another_format_hex( TestObjs *objs ) {
  fixpoint_str_t string;

  fixpoint_t value;
  TEST_FIXPOINT_INIT(&value, 0, 0x40000000, false); // this is 0.4
  fixpoint_format_hex(&string, &value);

  ASSERT(strcmp(string.str, "0.4") == 0);

  // this is for negative integer
  fixpoint_format_hex(&string, &objs->neg_eleven);
  ASSERT(strcmp(string.str, "-b.0") == 0);
}

void test_another_parse_hex( TestObjs *objs ) {
  fixpoint_t value;

  // check if too many digitss
  ASSERT(false == fixpoint_parse_hex(&value, FIXPOINT_STR("123456789.0")));

  // check for invalid characters
  ASSERT(false == fixpoint_parse_hex(&value, FIXPOINT_STR("1G.0")));

  // check for any possible trailing junk around
  ASSERT(false == fixpoint_parse_hex(&value, FIXPOINT_STR("1.0xyz")));

  // here you could have an empty string
  ASSERT(false == fixpoint_parse_hex(&value, FIXPOINT_STR("")));
}
