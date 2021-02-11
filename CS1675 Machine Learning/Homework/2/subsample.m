function [newdata] = subsample(data, k)
%Return a random sample of k elements from the data
newdata = randsample(data,k)
end

