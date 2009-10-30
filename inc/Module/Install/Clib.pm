#line 1
package Module::Install::Clib;
use strict;
use warnings;
our $VERSION = '0.04';
use 5.008_001;
use base qw(Module::Install::Base);
use Config;
use File::Spec;

sub clib_header {
    my ($self, $filename) = @_;
    (my $distname = $self->name) =~ s/Clib-//;

    my $blib_dir = File::Spec->catfile('$(INST_ARCHLIB)', 'auto', 'Clib', 'include', $distname);
    my $blib_dst = File::Spec->catdir($blib_dir, $filename);
$self->postamble(<<"END_MAKEFILE");
config :: $filename
\t\t\$(NOECHO) \$(ECHO) copy $filename to $blib_dst
\t\t\$(NOECHO) \$(MKPATH) $blib_dir
\t\t\$(NOECHO) \$(CP) $filename $blib_dst

END_MAKEFILE
}

sub clib_library {
    my ($self, $filename) = @_;
    (my $distname = $self->name) =~ s/Clib-//;

    my $blib_dir = File::Spec->catdir('$(INST_ARCHLIB)', 'auto', 'Clib', 'lib');
    my $blib_dst = File::Spec->catfile($blib_dir, $filename);
$self->postamble(<<"END_MAKEFILE");
config :: $filename
\t\t\$(NOECHO) \$(ECHO) copy $filename to $blib_dst
\t\t\$(NOECHO) \$(MKPATH) $blib_dir
\t\t\$(NOECHO) \$(CP) $filename $blib_dst

END_MAKEFILE
}

sub clib_setup {
    my ($self) = @_;
    my %uniq;
    my @dirs = grep { $uniq{$_}++ == 0 } map { File::Spec->catfile($_, qw/auto Clib/) } grep /$Config{archname}/, @INC;
    my @libs = grep { -d $_ } map { File::Spec->catfile($_, 'lib') }     @dirs;
    my @incs = grep { -d $_ } map { File::Spec->catfile($_, 'include') } @dirs;

    my $incs = $self->makemaker_args->{INC} || '';
    $self->makemaker_args->{INC} = "-I\$(INST_ARCHLIB)/auto/Clib/include/ " . join(" ", map { "-I$_" } @incs) . ' ' . $incs;

    my $libs = $self->makemaker_args->{LIBS} || '';
    $self->makemaker_args->{LIBS} = join(" ", map { "-L$_" } @libs) . ' ' . $libs;
}

1;
__END__

#line 113
