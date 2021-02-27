#!/bin/sh  
DIR=exp_double1

#tar -xvf vesicle_input3000_withdelete_opti0/archive.tar -C vesicle_input3000_withdelete_opti0/
#tar -xvf vesicle_input3000_withdelete_opti1/archive.tar -C vesicle_input3000_withdelete_opti1/
#tar -xvf vesicle_input3000_withdelete_opti2/archive.tar -C vesicle_input3000_withdelete_opti2/

tar -zxvf $DIR/archive.tar.gz -C $DIR/


#mv vesicle_input3000_withdelete_opti0/output/* vesicle_input3000_withdelete_opti0/
#mv vesicle_input3000_withdelete_opti1/output/* vesicle_input3000_withdelete_opti1/
#mv vesicle_input3000_withdelete_opti2/output/* vesicle_input3000_withdelete_opti2/


mv $DIR/output/* $DIR/
