function main_LR_SGD_v3()
    train = readmatrix("housing_train.txt");
    test = readmatrix("housing_test.txt");
    
    %training set
    [mean_norm_train, std_norm_train] = compute_norm_parameters(train(:,1:13));
    x_norm_train = normalize(train(:,1:13),mean_norm_train, std_norm_train);
    biasCol = ones(size(x_norm_train,1),1);
    x_norm_train = [biasCol x_norm_train];
    
    %testing set
    [mean_norm_test, std_norm_test] = compute_norm_parameters(test(:,1:13));
    x_norm_test = normalize(test(:,1:13),mean_norm_test, std_norm_test);
    biasCol = ones(size(x_norm_test,1),1);
    x_norm_test = [biasCol  x_norm_test];
    
    w = ones(size(x_norm_train,2),1);
    
    p = init_progress_graph;
    
    for t = 1:1000
        if mod(t,50) == 0
            %apply to train data
            train_prediction = LR_predict(x_norm_train(:,:), w);
            train_error = train_prediction - train(:,14);
            train_mse = mean(train_error.^2);
            %apply to test data
            test_prediction = LR_predict(x_norm_test(:,:), w);
            test_error = test_prediction - test(:,14);
            test_mse = mean(test_error.^2);
            
            p = add_to_progress_graph(p, t, train_mse, test_mse);
        end
        
        rate = 0.05/t;
        row = mod(t-1,size(train,1))+1;
        
        y = train(row, 14);
        x_i = x_norm_train(row, :);
        f_x = x_i * w;
        
        
        %iterate w
        w = w + rate*(y - f_x).*x_i.';
    end
    
    disp("weights");
    disp(w);
    
    %apply to train data
    train_prediction = LR_predict(x_norm_train(:,:), w);
    train_error = train_prediction - train(:,14);
    train_mse = mean(train_error.^2);
    
    disp("training data mean squared error =");
    disp(train_mse);
    disp("");
    
    
    %apply to test data
    test_prediction = LR_predict(x_norm_test(:,:), w);
    test_error = test_prediction - test(:,14);
    test_mse = mean(test_error.^2);
    
    disp("testing data mean squared error =");
    disp(test_mse);
    disp("");

end