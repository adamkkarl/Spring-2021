function [train test] = kfold_crossvalidation(data,k,m)
%splits the data into train and test groups
%note: if k is 10, m should be from 0 through 9
train = []
test = []
for i = 1:length(data)
    if mod(i,k) == m
        test = [test data(i)]
    else
        train = [train data(i)]
    end
end

