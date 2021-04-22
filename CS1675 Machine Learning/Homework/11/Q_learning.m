function Q_learning

RL_init_model();
state = 0;

action = 0;
step = 0;
gamma = 0.95;

n_sa =[0,0,0; 0,0,0; 0,0,0; 0,0,0];
Q    =[0,0,0; 0,0,0; 0,0,0; 0,0,0];

for i=1:15000
    if mod(step, 200) == 0
        state = RL_reset_environment(model);
    end
    
    %choose action
    p = [0.2, 0.2, 0.2];
    max_action = find(Q(state,:) == max(Q(state,:)), 1);
    p(max_action) = 0.6;
    disp("===");
    disp(p);
    X = rand * sum(p); %account for ties
    if X < p(1)
        action = 1;
    elseif X < p(1) + p(2)
        action = 2;
    else
        action = 3;
    end

    [new_state, reward] = RL_simulate_one_step(state, action, model);
    
    n_sa(state, action) = n_sa(state, action) + 1;
    alpha = 1/((1+n_sa(state, action))^0.6);
    Q(state, action) = (1-alpha)*Q(state, action) + alpha*(reward+gamma*max(Q(new_state,:)));
    
    state = new_state;
    step = step+1;
end
    V = max(Q,[],2);
    pi = [];
    for i=1:size(Q,1)
        pi = [pi, find(Q(i,:)==V(i,1), 1)];
    end

    disp("Q");
    disp(Q);
    disp("policy");
    disp(pi);
    disp("V");
    disp(V);
end