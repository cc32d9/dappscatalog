use strict;
use warnings;
use File::Temp;
use IO::Dir;
use File::Copy;
use IO::File;



my $infile = shift @ARGV;

my $abifile = $infile;
$abifile =~ s/\.cpp/.abi/;

my @copyfiles = ('\.hpp$', '_rc\.md$');

my $tmpdir = File::Temp->newdir(CLEANUP => 0);
my $tmpdirname = $tmpdir->dirname();

my $d = IO::Dir->new(".");

while( defined(my $f = $d->read) )
{
    if( grep {$f =~ $_} @copyfiles )
    {
        if( $f =~ /\.hpp$/ )
        {
            print "Copying $f to $tmpdirname and removing ABI actions\n";
            my $in = IO::File->new($f) or die($!);
            my $out = IO::File->new($tmpdirname . '/' . $f, 'w') or die($!);
            
            while( my $line = $in->getline )
            {
                $line =~ s,\/\/\/\s*\@abi\s+action,,;
                $out->print($line);
            }
        }
        else
        {
            print "Copying $f to $tmpdirname\n";
            copy($f, $tmpdirname);
        }
    }
}


my $in = IO::File->new($infile) or die($!);
my $out = IO::File->new($tmpdirname . '/' . $infile, 'w') or die($!);

while( (my $line = $in->getline) !~ /void\s+apply/ )
{
    $out->print($line);
}

my $started;
while( my $line = $in->getline )
{
    if( $line =~ /EOSIO_API/ )
    {
        $started = 1;
        $line =~ s/EOSIO_API/EOSIO_ABI/;
        $out->print($line);
        last;
    }
}

print "copied $infile to $tmpdirname with modifications\n";

system("cd $tmpdirname; eosiocpp -g $abifile $infile") == 0 or die($!);

copy($tmpdirname . '/' . $abifile, '.') or die($!);





