%%% using the input vector, weights, and bias, calculate
%%% the class decision

function [decisions] = apply_svlm(x, w, b)

decisions = [];
for i = 1:size(x, 1)
    r = x(i, :);

    d = 0;
    temp = r*w + b;
    if temp >= 0
        d = 1;
    end
    decisions = [decisions; temp];
end