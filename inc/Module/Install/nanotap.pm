#line 1
use warnings;
use strict;

package Module::Install::nanotap;
use Module::Install::Base;
our @ISA     = qw(Module::Install::Base);
use Config;
use File::Spec;

sub use_nanotap {
    my ($self) = @_;

    $self->clib_setup();

    my ($header) = grep { -f $_ } map {
        File::Spec->catfile( $_, 'auto', 'Clib', 'include', 'nanotap',
            'nanotap.h' )
    } grep /$Config{archname}/, @INC;
    if ($header) {
        $self->admin->copy($header, File::Spec->catfile(qw/inc auto Clib include nanotap nanotap.h/));
        my $incs = $self->makemaker_args->{INC} || '';
        $self->makemaker_args->{INC} = "-Iinc/auto/Clib/include/ $incs";
    } else {
        die "cannot find nanotap.h";
    }
}

1;
__END__

#line 60
