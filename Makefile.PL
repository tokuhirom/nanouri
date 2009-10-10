use inc::Module::Install;

my $e = env_for_c(CPPPATH => 'extlib');
$e->enable_warnings;
$e->test('t/01_parse', 't/01_parse.c');
$e->test('t/02_escape', 't/02_escape.cc');

tests 't/*.t';

WriteMakefileForC;
