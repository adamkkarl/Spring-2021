function [V_pi_hist] = main_1c

RL_init_model();
state = 0;

policy = [1, 1, 3, 2];

hist = [];
action = 0;
step = 0;
gamma = 0.95;
V_pi = [0,0,0,0];
V_pi_hist = [];
n_s = [0,0,0,0];

for i=1:15000
    if mod(step, 100) == 0
        %record V_pi
        V_pi_hist = [V_pi_hist; step, V_pi];
    end
    if mod(step, 200) == 0
        %reset model
        state = RL_reset_environment(model);
    end

    action = policy(state);
    n_s(state) = n_s(state) + 1;
    
    [new_state, reward] = RL_simulate_one_step(state, action, model);
    
    alpha = 1/((1+n_s(state))^0.6);
    
    V_pi(state) = (1-alpha)*V_pi(state) + alpha*(reward+gamma*V_pi(new_state));
    
    disp(V_pi);
    state = new_state;
    step = step+1;
end
    V_pi_hist = [V_pi_hist; step, V_pi];
end