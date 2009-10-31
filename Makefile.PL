use inc::Module::Install;
name 'nanouri';
all_from 'lib/Clib/nanouri.pm';
license 'perl';

clib_setup;

my $e = env_for_c(CPPPATH => 'extlib');
$e->enable_warnings;
$e->test('t/01_parse', 't/01_parse.c');
$e->test('t/02_escape', 't/02_escape.cc');
$e->test('t/03_class', 't/03_class.cc');
$e->test('t/04_unescape', 't/04_unescape.cc');

tests 't/*.t';

use_nanotap();
WriteMakefile;
