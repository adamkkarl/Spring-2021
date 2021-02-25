function w = LR_solve(X, y)
%calculate a vector of weights with the minimal square fit
    w = X\y;
end