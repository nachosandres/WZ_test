#!/bin/bash

hadd -f susy_SR01_lowpt.root susy_SR01_*_lowpt.root
hadd -f susy_SR11_lowpt.root susy_SR11_*_lowpt.root
hadd -f susy_SR21_lowpt.root susy_SR21_*_lowpt.root
hadd -f susy_lowpt.root susy_SR??_lowpt.root
hadd -f susy_SR01_highpt.root susy_SR01_*_highpt.root
hadd -f susy_SR11_highpt.root susy_SR11_*_highpt.root
hadd -f susy_SR21_highpt.root susy_SR21_*_highpt.root
hadd -f susy_highpt.root susy_SR??_highpt.root
