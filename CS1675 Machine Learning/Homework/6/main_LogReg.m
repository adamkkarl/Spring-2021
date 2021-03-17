function [train_pred, test_pred] = main_LogReg()

    train = readmatrix("pima_train.txt");
    test = readmatrix("pima_test.txt");
    
    % normalize training inputs
    [train_mean, train_std] = compute_norm_parameters(train);
    x_norm_train = normalize(train(:,1:8), train_mean, train_std);
    
    % normalize testing inputs based on training params
    x_norm_test = normalize(test(:,1:8), train_mean, train_std);
    
    % run gradient descent on trining data for 2000 iterations
    W = Log_regression(x_norm_train, train(:,9), 2000);
    
    
    % add a column of ones on the left of x_norm_train
    col_ones = ones(size(x_norm_train, 1), 1);
    x_norm_train = horzcat(col_ones, x_norm_train);

    %make into binary prediction
    train_pred = x_norm_train * W;

    
        
    % add a column of ones on the left of x_norm_test
    col_ones = ones(size(x_norm_test, 1), 1);
    x_norm_test = horzcat(col_ones, x_norm_test);
    
    %make into binary prediction
    test_pred = x_norm_test * W;
   
end