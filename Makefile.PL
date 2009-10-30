use inc::Module::Install;
name 'nanouri';
all_from 'lib/Clib/nanouri.pm';
license 'perl';

clib_setup;
use_nanotap();
use Data::Dumper; warn Dumper(makemaker_args);

my $e = env_for_c(CPPPATH => 'extlib');
use Data::Dumper; warn Dumper($e->{INC});
$e->enable_warnings;
$e->test('t/01_parse', 't/01_parse.c');
$e->test('t/02_escape', 't/02_escape.cc');
$e->test('t/03_class', 't/03_class.cc');

tests 't/*.t';

WriteMakefile;
