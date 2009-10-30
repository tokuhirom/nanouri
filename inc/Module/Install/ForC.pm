#line 1
package Module::Install::ForC;
use strict;
use warnings;
our $VERSION = '0.20';
use 5.008000;
use Module::Install::ForC::Env;
use Config;              # first released with perl 5.00307
use File::Basename ();   # first released with perl 5
use FindBin;             # first released with perl 5.00307

use Module::Install::Base;
our @ISA     = qw(Module::Install::Base);

our %OBJECTS;
our @CONFIG_H;

sub env_for_c {
    my $self = shift;
    $self->_forc_initialize();
    $self->admin->copy_package('Module::Install::ForC::Env');
    Module::Install::ForC::Env->new($self, @_)
}
sub is_linux () { $^O eq 'linux'  }
sub is_mac   () { $^O eq 'darwin' }
sub is_win32 () { $^O eq 'MSWin32' }

# (DEPRECATED)
sub WriteMakefileForC { shift->WriteMakefile(@_) }

sub WriteHeaderForC {
    my ($self, $fname) = @_;
    $fname or die "Usage: WriteHeaderForC('foo_config.h')";

    (my $guard = $fname) =~ tr{a-z./\055}{A-Z___};

    my $header = "#ifndef $guard\n"
               . "#define $guard\n\n";
    my $footer = "\n\n#endif  /* $guard */\n";
    if (my $version = $self->version) {
        (my $verkey = $self->name) =~ s/^Clib-//;
        $verkey =~ tr{a-z./\055}{A-Z___};
        $verkey .= "_VERSION";
        $header .= qq{#define $verkey "$version"\n\n};
    }

    open my $fh, '>', $fname or die "cannot open file($fname): $!";
    print $fh $header . join('', @CONFIG_H) . $footer;
    close $fh;
}

{
    my $initialized = 0;
    sub _forc_initialize {
        return if $initialized++;

        my $self = shift;
        $self->makemaker_args(
            # linking, compiling is job for ForC.
            C      => [],
            OBJECT => '',
        );
    }
}

1;
__END__

#line 157
