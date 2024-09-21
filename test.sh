#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
#Empty File input:
printf "" > test_files/empty.input

#-------------------------------ASCII File Input-----------------------------:
printf "Hello, World!\n" > test_files/ascii1.input
printf "Hello, World!" > test_files/ascii2.input
printf "\x26\x27\x28" > test_files/ascii3.input                    # &'(
printf "\x48\x61\x70\x70\x79" > test_files/ascii4.input            # Happy


#-------------------------------ISO File Input-------------------------------:
#1.Start with ASCII, and end with ISO
printf "Hello \xe6 \xf8 \xe5\n" > test_files/ISO1_MixASC.input 

#2.Pure ISO file
printf "\xe6 \xf8 \xe5\n" > test_files/ISO2.input             #æ ø å in iso-8859-1

#3.Pure ISO file
printf "\xc4 \xa5 \xe1 \e2" > test_files/ISO3.input           

#4.Start with ASCII, then ISO, then end with ASCII:
printf "Hello \xe6 \xf8 \xe5 World!\n" > test_files/ISO4.input 

#5.Start with ISO, then end with ASCII:
printf "\xe6 \xf8 \xe5 ISO File\n" > test_files/ISO5.input 

#6.Start with ASCII, end with ISO:
printf "Is this an ISO? \xc4 \xa5 \xe1 \e2" > test_files/ISO6.input 


#-------------------------------UTF File Input-------------------------------:
#1. Ý, pure UTF encoding:
printf "\xc3\x9d\n" > test_files/UTF1.input  

#2. UTFæ start with ASCII encoding and end with only one UTF encoding:
printf "UTF\xc3\x86\n" > test_files/UTF2_MixASC.input 

#3. UTFæÆÆÆ start with ASCII encoding and end with more UTF encoding:
printf "UTF\xc3\xa6\xc3\x86\xc3\x86\xc3\x86\n" > test_files/UTF3_MixASC.input 

#4. Start with "\xc4 \xa5 \xe1 \e2", which is ISO encoding, and then end with one UTF encoding "\xc3 \c9d"
printf "\xc4 \xa5 \xe1 \e2 \xc3 \c9d\n" > test_files/UTF4_MixISO.input

#5. Start with ASCII, UTF in the middle and then end with ISO
printf "Hello \xc3 \c9 \xc4 \xa5 \xe1 \e2 d\n" > test_files/UTF5_MixISOASC.input

#6. Start with ASCII "Happy" and ISO "\xc4 \xa5 \xe1 \e2", UTF "\xc3 \c9d" in the end:
printf "Happy \xc4 \xa5 \xe1 \e2 \xc3 \c9d\n" > test_files/UTF6_MixASCISO.input

#7. Start with UTF, then ISO, then end with ASCII:
printf "\xc3 \c9d \xc4 \xa5 \xe1 \e2 \n Is this UTF?" > test_files/UTF7.input



#-------------------------------DATA File Input-------------------------------:
printf "Hello,\x00World!\n" > test_files/DATA1_MixASC.input
printf "\xe6 \xf8 \x00 \xe5\n" > test_files/DATA2_MixISO.input
printf "Hello \x00 \xe6 \xf8 \x00 \xe5\n" > test_files/DATA3_MixASCISO.input
printf "Happy \xc4 \xa5 \xe1 \e2 \xc3 \c9d\n \x00" > test_files/DATA4_MixASCISOUTF.input

#--------------Test when the input file is non-readable--------------:
printf "hiddenFile" > test_files/hidden_file
chmod -r test_files/hidden_file
./file test_files/hidden_file


echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    ${f} | sed -e 's/ASCII text.*/ASCII text/' \
                         -e 's/UTF-8 Unicode text.*/UTF-8 Unicode text/' \
                         -e 's/ISO-8859 text.*/ISO-8859 text/' \
                         -e 's/writable, regular file, no read permission/cannot determine (Permission denied)/' \
                         > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done

#--------------Test when the input file is non-existing--------------:

echo "-------------Test result for non-existing file: -------------"
file  nonExisting | sed -e 's/cannot open.*/cannot determine (No such file or directory)/'\
                    > "test_files/non-existing_expected"
./file nonExisting > "test_files/non-exising_actual"
if ! diff -u "non-existing_expected" "non-exising_actual"
then
    echo ">>> Failed :-("
else
    echo ">>> Success :-)"

fi

exit $exitcode