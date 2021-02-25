function main_LR()
%driver method for linear regression testing
    train = readmatrix("housing_train.txt");
    test = readmatrix("housing_test.txt");
    w = LR_solve(train(:,1:13), train(:,14));
    
    disp("weights:");
    disp(w);
    disp("");
    
    
    %apply to train data
    train_prediction = LR_predict(train(:,1:13), w);
    train_error = train_prediction - train(:,14);
    train_mse = mean(train_error.^2);
    
    disp("training data mean squared error =");
    disp(train_mse);
    disp("");
    
    
    
    %apply to test data
    test_prediction = LR_predict(test(:,1:13), w);
    test_error = test_prediction - test(:,14);
    test_mse = mean(test_error.^2);
    
    disp("testing data mean squared error =");
    disp(test_mse);
    disp("");
    
end