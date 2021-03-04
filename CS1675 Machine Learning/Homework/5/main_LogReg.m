function main_LogReg()

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
    for i=1:size(x_norm_train, 1)
        if train_pred(i,1) < 0
            train_pred(i,1) = 0;
        else 
            train_pred(i,1) = 1;
        end
    end
    
    train_conf_mat = confusionmat(train_pred, train(:,9));
    train_conf_mat = train_conf_mat/size(x_norm_train, 1);
    
    disp("training confusion matrix");
    disp(train_conf_mat);
    
    train_E = train_conf_mat(2,1) + train_conf_mat(1,2);
    disp("training misclassification error")
    disp(train_E);
    
        
    % add a column of ones on the left of x_norm_test
    col_ones = ones(size(x_norm_test, 1), 1);
    x_norm_test = horzcat(col_ones, x_norm_test);
    
    %make into binary prediction
    test_pred = x_norm_test * W;
    for i=1:size(x_norm_test, 1)
        if test_pred(i,1) < 0
            test_pred(i,1) = 0;
        else 
            test_pred(i,1) = 1;
        end
    end

    
    test_conf_mat = confusionmat(test_pred, test(:,9));
    test_conf_mat = test_conf_mat/size(x_norm_test, 1);
    
    disp("testing confusion matrix");
    disp(test_conf_mat);
    
    test_E = test_conf_mat(2,1) + test_conf_mat(1,2);
    disp("testing misclassification error")
    disp(test_E);
    
    sens = test_conf_mat(1,1) / (test_conf_mat(1,1) + test_conf_mat(2,1));
    disp("Testing sensitivity");
    disp(sens);
    
    spec = test_conf_mat(2,2) / (test_conf_mat(2,2) + test_conf_mat(1,2));
    disp("Testing specificity");
    disp(spec);
end