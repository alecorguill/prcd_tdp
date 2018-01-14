#!/bin/sh
#SBATCH --job-name=nbody
#SBATCH --output=nbody.txt
#SBATCH --output=batch.out
#SBATCH --err=batch.err
#SBATCH --time=1:00:00
#SBATCH -p mistral
#SBATCH -N 4 
  
source test.sh
