function [S] = main_2_2a
    [train_img, train_label, test_img, test_label] = data_loader('data/classification');

    S = randi(446, [1,9]);
    disp(S);
    show_multiple_images(train_img(:,S));

end