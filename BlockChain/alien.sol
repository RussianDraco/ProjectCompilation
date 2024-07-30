// followed tutorial at https://medium.com/@solidity101/solidity-for-dummies-a-beginners-guide-to-smart-contract-development-56d0fd346b97

pragma solidity ^0.8.0;
contract AlienToken {
    string public name = "Alien Token";
    string public symbol = "AT";
    uint256 public totalSupply = 1000000 * 10**18;
    mapping(address => uint256) public balanceOf;
    constructor() {
        balanceOf[msg.sender] = totalSupply;
    }
    event Transfer(address indexed from, address indexed to, uint256 value);
    function transfer(address _to, uint256 _value) public returns (bool success) {
        require(_to != address(0), "Invalid address");
        require(balanceOf[msg.sender] >= _value, "Insufficient balance");
        balanceOf[msg.sender] -= _value;
        balanceOf[_to] += _value;
        emit Transfer(msg.sender, _to, _value);
        return true;
    }
}